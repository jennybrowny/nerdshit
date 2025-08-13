#include "states/save_state.hpp"
#include "states/start_state.hpp"
#include "states/ACT1_state.hpp"
#include "states/tutorial_state.hpp" 
#include "game.hpp"
#include "audio_manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

// Static color definitions
const sf::Color SaveState::backgroundColor = sf::Color(15, 20, 30);
const sf::Color SaveState::panelColor = sf::Color(25, 35, 50, 220);
const sf::Color SaveState::selectedColor = sf::Color(80, 120, 200, 180);
const sf::Color SaveState::hoveredColor = sf::Color(50, 80, 120, 120);
const sf::Color SaveState::textColor = sf::Color(220, 220, 220);
const sf::Color SaveState::emptySlotColor = sf::Color(40, 45, 55, 100);


// Make slots for four games, with UI
SaveState::SaveState(sf::Font& font) 
    : font(font), selectedSlot(-1), hoveredSlot(-1),
      titleText(font), instructionText(font)
{
    // Load audio resources first
    auto& audio = AudioManager::getInstance();
    try {
        audio.loadSound("click", "assets/sounds/button_click.mp3");
        audio.loadSound("sparkle", "assets/sounds/sparkle.mp3");
    } catch (...) {
        std::cout << "Warning: Could not load audio files for SaveState\n";
    }
    
    // Create main panel
    mainPanel.setSize(sf::Vector2f(600, 450));
    mainPanel.setPosition(sf::Vector2f(100, 75));
    mainPanel.setFillColor(panelColor);
    mainPanel.setOutlineThickness(3);
    mainPanel.setOutlineColor(sf::Color(60, 80, 120));
    
    // Title background
    titleBackground.setSize(sf::Vector2f(600, 60));
    titleBackground.setPosition(sf::Vector2f(100, 75));
    titleBackground.setFillColor(sf::Color(35, 45, 65));
    titleBackground.setOutlineThickness(2);
    titleBackground.setOutlineColor(sf::Color(80, 100, 140));
    
    // Title text 
    titleText.setString("SAVE GAME MANAGER");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color(150, 200, 255));
    titleText.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(sf::Vector2f(
        titleBounds.position.x + titleBounds.size.x / 2.0f,
        titleBounds.position.y + titleBounds.size.y / 2.0f
    ));
    titleText.setPosition(sf::Vector2f(400, 105));
    
    // Instruction text 
    instructionText.setString("Manage your saves");
    instructionText.setCharacterSize(16);
    instructionText.setFillColor(sf::Color(180, 180, 180));
    sf::FloatRect instrBounds = instructionText.getLocalBounds();
    instructionText.setOrigin(sf::Vector2f(
        instrBounds.position.x + instrBounds.size.x / 2.0f,
        instrBounds.position.y + instrBounds.size.y / 2.0f
    ));
    instructionText.setPosition(sf::Vector2f(400, 150));
    
    // Buttons UI
    backButton = std::make_unique<Button>(
        sf::Vector2f(120, 45),
        sf::Vector2f(150, 470),
        sf::Color(60, 60, 70),
        font,
        "BACK"
    );
    
    loadButton = std::make_unique<Button>(
        sf::Vector2f(140, 45),
        sf::Vector2f(300, 470),
        sf::Color(50, 120, 50),
        font,
        "LOAD GAME"
    );    
    newGameButton = std::make_unique<Button>(
        sf::Vector2f(140, 45),
        sf::Vector2f(470, 470),
        sf::Color(50, 150, 80),
        font,
        "NEW GAME"
    );
    
    // Clear All Saves button
    clearAllButton = std::make_unique<Button>(
        sf::Vector2f(120, 35),
        sf::Vector2f(650, 150),
        sf::Color(180, 60, 60),
        font,
        "CLEAR ALL"
    );
    
    loadSaveSlots();
    createSlotVisuals();
}

// Get four saved slots and draw that shit
void SaveState::loadSaveSlots() {
    saveSlots.clear();
    saveSlots.reserve(4); // Pre-allocate to avoid reallocations
    
    for (int i = 1; i <= 4; i++) {
        // Construct directly in vector to avoid copying
        saveSlots.emplace_back();
        auto& slot = saveSlots.back();
        
        slot.slotNumber = i;
        slot.exists = false;
        slot.isSelected = false;
        
        std::string saveFile = "savegame_slot" + std::to_string(i) + ".dat";
        std::ifstream file(saveFile);
        
        if (file.good()) {
            int act, scene;
            std::time_t saveTime;
            if (file >> act >> scene >> saveTime) {
                slot.exists = true;
                slot.act = act;
                slot.scene = scene;
                slot.saveTime = saveTime;
                
                std::ostringstream oss;
                oss << "Slot " << i << ": ACT " << act << " - Scene " << scene;
                slot.displayText = oss.str();
                
                char timeBuffer[100];
                std::strftime(timeBuffer, sizeof(timeBuffer), "Saved: %m/%d/%Y at %H:%M", 
                             std::localtime(&saveTime));
                slot.timeText = timeBuffer;
            }
        }
        
        if (!slot.exists) {
            slot.displayText = "Slot " + std::to_string(i) + ": Empty";
            slot.timeText = "Click to save new game here";
        }
    }
}
// Make it NOT ugly the slots
void SaveState::createSlotVisuals() {
    for (size_t i = 0; i < saveSlots.size(); i++) {
        auto& slot = saveSlots[i];
        
        // Background shape for each slot (made slightly narrower to accommodate individual Clear buttons)
        sf::Vector2f slotPos(130, 180 + i * 65);
        slot.backgroundShape.setSize(sf::Vector2f(480, 55)); // Reduced width from 540 to 480
        slot.backgroundShape.setPosition(slotPos);
        slot.backgroundShape.setOutlineThickness(2);
        
        // Selection outline (initially invisible)
        slot.selectionOutline.setSize(sf::Vector2f(484, 59));
        slot.selectionOutline.setPosition(sf::Vector2f(slotPos.x - 2, slotPos.y - 2));
        slot.selectionOutline.setFillColor(sf::Color::Transparent);
        slot.selectionOutline.setOutlineThickness(3);
        slot.selectionOutline.setOutlineColor(sf::Color(100, 150, 255));
        
        // Create sf::Text objects with font reference
        slot.mainText = std::make_unique<sf::Text>(font);
        slot.mainText->setString(slot.displayText);
        slot.mainText->setCharacterSize(18);
        slot.mainText->setPosition(sf::Vector2f(slotPos.x + 15, slotPos.y + 8));
        
        slot.timeDisplay = std::make_unique<sf::Text>(font);
        slot.timeDisplay->setString(slot.timeText);
        slot.timeDisplay->setCharacterSize(14);
        slot.timeDisplay->setPosition(sf::Vector2f(slotPos.x + 15, slotPos.y + 30));
        
        // Individual Clear button for each slot (only if slot exists)
        if (slot.exists) {
            slot.clearButton = std::make_unique<Button>(
                sf::Vector2f(70, 35),
                sf::Vector2f(slotPos.x + 490, slotPos.y + 10), // Position to the right of slot
                sf::Color(180, 60, 60),
                font,
                "CLEAR"
            );
        } else {
            slot.clearButton.reset(); // No clear button for empty slots
        }
        
        // Set colors based on slot state
        if (slot.exists) {
            slot.backgroundShape.setFillColor(sf::Color(35, 50, 70, 150));
            slot.backgroundShape.setOutlineColor(sf::Color(70, 90, 120));
            slot.mainText->setFillColor(textColor);
            slot.timeDisplay->setFillColor(sf::Color(160, 160, 160));
        } else {
            slot.backgroundShape.setFillColor(emptySlotColor);
            slot.backgroundShape.setOutlineColor(sf::Color(60, 65, 75));
            slot.mainText->setFillColor(sf::Color(120, 120, 120));
            slot.timeDisplay->setFillColor(sf::Color(100, 100, 100));
        }
    }
}

void SaveState::updateSlotSelection(const sf::Vector2f& mousePos) {
    hoveredSlot = -1;
    
    for (size_t i = 0; i < saveSlots.size(); i++) {
        sf::FloatRect slotBounds = saveSlots[i].backgroundShape.getGlobalBounds();
        
        if (slotBounds.contains(mousePos)) {
            hoveredSlot = static_cast<int>(i);
            
            // Update visual feedback
            saveSlots[i].backgroundShape.setFillColor(hoveredColor);
            if (saveSlots[i].exists) {
                saveSlots[i].backgroundShape.setOutlineColor(sf::Color(120, 140, 180));
            }
        } else {
            // Reset to normal colors
            if (saveSlots[i].exists) {
                saveSlots[i].backgroundShape.setFillColor(sf::Color(35, 50, 70, 150));
                saveSlots[i].backgroundShape.setOutlineColor(sf::Color(70, 90, 120));
            } else {
                saveSlots[i].backgroundShape.setFillColor(emptySlotColor);
                saveSlots[i].backgroundShape.setOutlineColor(sf::Color(60, 65, 75));
            }
        }
        
        // Show selection outline
        if (static_cast<int>(i) == selectedSlot) {
            saveSlots[i].backgroundShape.setFillColor(selectedColor);
            saveSlots[i].selectionOutline.setOutlineColor(sf::Color(100, 150, 255, 200));
        } else {
            saveSlots[i].selectionOutline.setOutlineColor(sf::Color::Transparent);
        }
        
        // Update individual Clear button hover states
        if (saveSlots[i].clearButton) {
            saveSlots[i].clearButton->update(mousePos);
        }
    }
}

void SaveState::handleEvents(Game& game) {
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow()));
    
    while (auto event = game.getWindow().pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            game.getWindow().close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            mousePos = game.getWindow().mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});
            
            // Check main button clicks (audio feedback handled in Button class)
            if (backButton->isClicked(mousePos)) {
                game.changeState(std::make_unique<StartState>(game.getFont()));
                return;
            }
            
            if (loadButton->isClicked(mousePos) && selectedSlot >= 0) {
                loadSelectedSave(game);
                return;
            }
            
            // REMOVED - Delete button click handling (no longer needed)
            
            if (newGameButton->isClicked(mousePos)) {
                // FIXED: Pass Game reference correctly instead of just game
                game.changeState(std::make_unique<TutorialState>(game));
                return;
            }
            
            // Clear All Saves button
            if (clearAllButton->isClicked(mousePos)) {
                clearAllSaves();
                return;
            }
            
            // Check individual Clear buttons for each slot
            for (size_t i = 0; i < saveSlots.size(); i++) {
                if (saveSlots[i].clearButton && saveSlots[i].clearButton->isClicked(mousePos)) {
                    clearSpecificSave(static_cast<int>(i));
                    return;
                }
            }
            
            // Check slot selection (clicking on the slot itself)
            for (size_t i = 0; i < saveSlots.size(); i++) {
                sf::FloatRect slotBounds = saveSlots[i].backgroundShape.getGlobalBounds();
                if (slotBounds.contains(mousePos)) {
                    selectedSlot = static_cast<int>(i);
                    std::cout << "Selected save slot " << (selectedSlot + 1) << "\n";
                    
                    // Play selection sound
                    try {
                        AudioManager::getInstance().playSound("click");
                    } catch (...) {
                        // Ignore audio errors
                    }
                    break;
                }
            }
        }
        else if (event->is<sf::Event::KeyPressed>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                game.changeState(std::make_unique<StartState>(game.getFont()));
                return;
            }
        }
    }
}

void SaveState::update(Game& game) {
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow()));
    
    // Main buttons 
    backButton->update(mousePos);
    loadButton->update(mousePos);
    newGameButton->update(mousePos);
    clearAllButton->update(mousePos);
    
    // Slot selection visuals (includes individual Clear button updates)
    updateSlotSelection(mousePos);
    
    // Button states based on selection 
    loadButton->setVisible(selectedSlot >= 0 && selectedSlot < static_cast<int>(saveSlots.size()) && saveSlots[selectedSlot].exists);
    
    // Show Clear All button only if there are saves to clear
    clearAllButton->setVisible(hasAnySaves());
}

// Render everything... like literally everything

void SaveState::render(Game& game) {
    // Clear with dark background
    game.getWindow().clear(backgroundColor);
    
    // Draw main panel
    game.getWindow().draw(mainPanel);
    
    // Draw title section
    game.getWindow().draw(titleBackground);
    game.getWindow().draw(titleText);
    game.getWindow().draw(instructionText);
    
    // Draw save slots
    for (const auto& slot : saveSlots) {
        game.getWindow().draw(slot.selectionOutline);  // Selection outline first
        game.getWindow().draw(slot.backgroundShape);
        if (slot.mainText) game.getWindow().draw(*slot.mainText);
        if (slot.timeDisplay) game.getWindow().draw(*slot.timeDisplay);
        
        // Draw individual Clear buttons
        if (slot.clearButton) {
            slot.clearButton->draw(game.getWindow());
        }
    }
    
    // Draw additional info for selected slot
    if (selectedSlot >= 0 && selectedSlot < static_cast<int>(saveSlots.size())) {
        sf::Text selectionInfo(font);
        if (saveSlots[selectedSlot].exists) {
            selectionInfo.setString("Selected: " + saveSlots[selectedSlot].displayText);
        } else {
            selectionInfo.setString("Selected: Empty Slot " + std::to_string(selectedSlot + 1));
        }
        selectionInfo.setCharacterSize(16);
        selectionInfo.setFillColor(sf::Color(150, 200, 255));
        selectionInfo.setPosition(sf::Vector2f(130, 440));
        game.getWindow().draw(selectionInfo);
    }
    
    // Draw main buttons
    backButton->draw(game.getWindow());
    if (loadButton->isVisible()) loadButton->draw(game.getWindow());
    newGameButton->draw(game.getWindow());
    if (clearAllButton->isVisible()) clearAllButton->draw(game.getWindow());
    
    // Draw instructions at bottom 
    sf::Text bottomInstructions(font);
    bottomInstructions.setString("ESC: Back to Menu | Click slots to select | Use buttons to manage saves");
    bottomInstructions.setCharacterSize(11);
    bottomInstructions.setFillColor(sf::Color(120, 120, 120));
    bottomInstructions.setPosition(sf::Vector2f(30, 540));
    game.getWindow().draw(bottomInstructions);
    
    game.getWindow().display();
}

// Function to load the saved game based on slot selection 

void SaveState::loadSelectedSave(Game& game) {
    if (selectedSlot >= 0 && selectedSlot < static_cast<int>(saveSlots.size()) && saveSlots[selectedSlot].exists) {
        const auto& slot = saveSlots[selectedSlot];
        std::cout << "Loading save: ACT" << slot.act << " Scene " << slot.scene << "\n";
        
        // Play success sound
        try {
            AudioManager::getInstance().playSound("sparkle");
        } catch (...) {
            // Ignore audio errors
        }
        
        // Load the specific save
        game.changeState(std::make_unique<ACT1_state>(game.getFont(), slot.scene - 1));
    }
}

// Clear all saves functionality just in case the player is so fucking laazy, optional tho..
void SaveState::clearAllSaves() {
    int clearedCount = 0;
    for (int i = 1; i <= 4; i++) {
        std::string saveFile = "savegame_slot" + std::to_string(i) + ".dat";
        if (std::filesystem::remove(saveFile)) {
            clearedCount++;
        }
    }
    
    if (clearedCount > 0) {
        std::cout << "Cleared " << clearedCount << " save file(s)." << std::endl;
        
        // Play success sound
        try {
            AudioManager::getInstance().playSound("sparkle");
        } catch (...) {
            // Ignore audio errors
        }
        
        loadSaveSlots();  // Reload to refresh the display
        createSlotVisuals();
        selectedSlot = -1;  // Clear selection
    } else {
        std::cout << "No save files found to clear." << std::endl;
    }
}

// Clear specific save functionality
void SaveState::clearSpecificSave(int slotIndex) {
    if (slotIndex >= 0 && slotIndex < static_cast<int>(saveSlots.size()) && saveSlots[slotIndex].exists) {
        std::string saveFile = "savegame_slot" + std::to_string(saveSlots[slotIndex].slotNumber) + ".dat";
        
        if (std::filesystem::remove(saveFile)) {
            std::cout << "Save slot " << (slotIndex + 1) << " cleared successfully." << std::endl;
            
            // Play deletion sound
            try {
                AudioManager::getInstance().playSound("click");
            } catch (...) {
                // Ignore audio errors
            }
            
            loadSaveSlots();  // Reload to refresh the display
            createSlotVisuals();
            
            // Clear selection if this was the selected slot
            if (selectedSlot == slotIndex) {
                selectedSlot = -1;
            }
        } else {
            std::cout << "Failed to delete save file: " << saveFile << std::endl;
        }
    }
}

bool SaveState::hasAnySaves() const {
    for (const auto& slot : saveSlots) {
        if (slot.exists) return true;
    }
    return false;
}