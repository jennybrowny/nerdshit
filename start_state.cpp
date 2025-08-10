#include <fstream> 
#include "game.hpp" 
#include <iostream>
#include <filesystem>
#include <sstream>
#include "states/start_state.hpp"
#include "states/credits_state.hpp"
#include "states/tutorial_state.hpp"
#include "states/ACT1_state.hpp"

StartState::StartState(sf::Font& font) : selectedSaveSlot(-1) {
    std::cout << "=== StartState Constructor Called ===" << std::endl;

    // menu buttons 
    try {
        std::cout << "Attempting to create start button..." << std::endl;
        startButton = std::make_unique<Button>(
            sf::Vector2f(200, 80),
            sf::Vector2f(300, 260),
            sf::Color::Green,
            font,
            "NEW GAME"
        );

        // credits button 
        creditsButton = std::make_unique<Button>(
            sf::Vector2f(200, 80),
            sf::Vector2f(300, 350),
            sf::Color::Transparent,
            font,
            "CREDITS"
        );
        creditsButton->setOutlineEnabled(false);
        creditsButton->setHoverColor(sf::Color::Yellow);
        
        // Delete saves button
        deleteButton = std::make_unique<Button>(
            sf::Vector2f(150, 50),
            sf::Vector2f(50, 500),
            sf::Color::Red,
            font,
            "Clear Saves"
        );
        
        std::cout << "Menu buttons created successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ERROR: Could not create buttons - " << e.what() << std::endl;
    } 
    
    loadSaveSlots();
    std::cout << "StartState constructor completed." << std::endl;
}

void StartState::loadSaveSlots() {
    saveSlots.clear();
    
    for (int i = 1; i <= 5; i++) {
        std::string saveFile = "savegame_slot" + std::to_string(i) + ".dat";
        SaveSlot slot;
        slot.slotNumber = i;
        slot.exists = false;
        
        std::ifstream file(saveFile);
        if (file.good()) {
            int act, scene;
            std::time_t saveTime;
            if (file >> act >> scene >> saveTime) {
                slot.exists = true;
                slot.act = act;
                slot.scene = scene;
                slot.saveTime = saveTime;
                
                // Format save info
                std::ostringstream oss;
                oss << "Slot " << i << ": ACT" << act << "_" << scene;
                slot.displayText = oss.str();
                
                // Format time
                char timeBuffer[100];
                std::strftime(timeBuffer, sizeof(timeBuffer), "%m/%d %H:%M", 
                             std::localtime(&saveTime));
                slot.timeText = timeBuffer;
            }
        }
        
        if (!slot.exists) {
            slot.displayText = "Slot " + std::to_string(i) + ": Empty";
            slot.timeText = "";
        }
        
        saveSlots.push_back(slot);
    }
}

void StartState::handleEvents(Game& game) {
    while (auto event = game.getWindow().pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            game.getWindow().close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            auto mousePos = game.getWindow().mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});

            if (startButton && startButton->isClicked(mousePos)) {
                std::cout << "START button clicked!" << std::endl;
                AudioManager::getInstance().playSound("click");
                game.changeState(std::make_unique<TutorialState>(game));
            }
            else if (creditsButton && creditsButton->isClicked(mousePos)) {
                std::cout << "CREDITS button clicked!" << std::endl;
                AudioManager::getInstance().playSound("click");
                game.changeState(std::make_unique<CreditsState>(game.getFont()));
            }
            else if (deleteButton && deleteButton->isClicked(mousePos)) {
                // Delete all save files
                for (int i = 1; i <= 5; i++) {
                    std::string saveFile = "savegame_slot" + std::to_string(i) + ".dat";
                    std::filesystem::remove(saveFile);
                }
                loadSaveSlots(); // Refresh the display
                AudioManager::getInstance().playSound("click");
                std::cout << "All saves cleared!" << std::endl;
            }
            else {
                // Check if clicked on a save slot
                for (int i = 0; i < saveSlots.size(); i++) {
                    sf::FloatRect slotRect(sf::Vector2f(50, 50 + i * 40), sf::Vector2f(300, 35));
                    if (slotRect.contains(mousePos) && saveSlots[i].exists) {
                        // Load this save
                        std::string saveFile = "savegame_slot" + std::to_string(saveSlots[i].slotNumber) + ".dat";
                        std::ifstream file(saveFile);
                        int act, scene;
                        std::time_t saveTime;
                        if (file >> act >> scene >> saveTime) {
                            game.changeState(std::make_unique<ACT1_state>(game.getFont(), scene-1));
                            AudioManager::getInstance().playSound("click");
                        }
                        break;
                    }
                }
            }
        }
        else if (event->is<sf::Event::KeyPressed>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                game.getWindow().close();
            }
        }
    }
}

void StartState::update(Game& game) {
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow()));
    
    if (startButton) startButton->update(mousePos);
    if (creditsButton) creditsButton->update(mousePos);
    if (deleteButton) deleteButton->update(mousePos);
    
    // Update selected save slot for hover effect
    selectedSaveSlot = -1;
    for (int i = 0; i < saveSlots.size(); i++) {
        sf::FloatRect slotRect(sf::Vector2f(50, 50 + i * 40), sf::Vector2f(300, 35));
        if (slotRect.contains(mousePos) && saveSlots[i].exists) {
            selectedSaveSlot = i;
            break;
        }
    }
}

void StartState::render(Game& game) {
    std::cout << "StartState::render called" << std::endl;
    
    // Clear the window first
    game.getWindow().clear(sf::Color::Black);

    // Draw title sprite if available
    if (game.getTitleSprite()) {
        std::cout << "Drawing title sprite" << std::endl;
        game.getWindow().draw(*game.getTitleSprite());
    } else {
        std::cout << "No title sprite - drawing fallback background" << std::endl;
        sf::RectangleShape background(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color(50, 100, 150));
        game.getWindow().draw(background);
    }

    // Draw save slots
    sf::Text saveTitle(game.getFont());
    saveTitle.setString("Saved Games:");
    saveTitle.setCharacterSize(24);
    saveTitle.setFillColor(sf::Color::White);
    saveTitle.setPosition(sf::Vector2f(50, 20));
    game.getWindow().draw(saveTitle);
    
    for (int i = 0; i < saveSlots.size(); i++) {
        sf::Vector2f slotPos(50, 50 + i * 40);
        
        // Draw slot background
        sf::RectangleShape slotBg(sf::Vector2f(300, 35));
        slotBg.setPosition(slotPos);
        
        if (saveSlots[i].exists) {
            if (selectedSaveSlot == i) {
                slotBg.setFillColor(sf::Color(100, 100, 200, 150)); // Hover effect
            } else {
                slotBg.setFillColor(sf::Color(50, 50, 50, 100));
            }
        } else {
            slotBg.setFillColor(sf::Color(30, 30, 30, 80));
        }
        
        game.getWindow().draw(slotBg);
        
        // Draw slot text
        sf::Text slotText(game.getFont());
        slotText.setString(saveSlots[i].displayText);
        slotText.setCharacterSize(18);
        slotText.setFillColor(saveSlots[i].exists ? sf::Color::White : sf::Color(128, 128, 128));
        slotText.setPosition(sf::Vector2f(slotPos.x + 5, slotPos.y + 5));
        game.getWindow().draw(slotText);
        
        // Draw time if save exists
        if (saveSlots[i].exists && !saveSlots[i].timeText.empty()) {
            sf::Text timeText(game.getFont());
            timeText.setString(saveSlots[i].timeText);
            timeText.setCharacterSize(14);
            timeText.setFillColor(sf::Color(200, 200, 200));
            timeText.setPosition(sf::Vector2f(slotPos.x + 200, slotPos.y + 15));
            game.getWindow().draw(timeText);
        }
    }

    // Draw instructions
    if (hasAnySaves()) {
        sf::Text instruction(game.getFont());
        instruction.setString("Click on a save slot to load");
        instruction.setCharacterSize(16);
        instruction.setFillColor(sf::Color::Yellow);
        instruction.setPosition(sf::Vector2f(50, 270));
        game.getWindow().draw(instruction);
    }

    // Draw buttons
    if (startButton) {
        std::cout << "Drawing start button" << std::endl;
        startButton->draw(game.getWindow());
    } 
    if (creditsButton) { 
        std::cout << "Drawing credits button" << std::endl;
        creditsButton->draw(game.getWindow());
    }
    if (deleteButton && hasAnySaves()) {
        std::cout << "Drawing delete button" << std::endl;
        deleteButton->draw(game.getWindow());
    }

    game.getWindow().display();
    std::cout << "StartState::render completed" << std::endl;
}

bool StartState::hasAnySaves() const {
    for (const auto& slot : saveSlots) {
        if (slot.exists) return true;
    }
    return false;
}