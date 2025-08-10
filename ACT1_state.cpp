#include "states/ACT1_state.hpp"
#include "game.hpp"
#include "resource_manager.hpp"
#include "states/start_state.hpp"
#include "audio_manager.hpp"
#include <iostream>
#include <filesystem>
#include "states/ACT1_state.hpp"


ACT1_state::ACT1_state(sf::Font& font, int startIndex) noexcept 
    : font(font),
      dummyTexture(),
      currentSprite(dummyTexture),
      currentIndex(startIndex),
      showSaveButton(startIndex == 0),  // Only show on first sprite
      justSaved(false),
      saveMessageTimer(0.0f)
{
    // Load all sprites first
    loadSprites();
    
    // Set to correct initial sprite if available
    if (!sprites.empty() && currentIndex < sprites.size()) {
        currentSprite.setTexture(sprites[currentIndex], true); // Reset rect
        std::cout << "Current sprite texture size: " 
                  << sprites[currentIndex].getSize().x << "x" 
                  << sprites[currentIndex].getSize().y << "\n";
    } else {
        std::cerr << "Warning: No sprites loaded or invalid start index\n";
        // Create a simple white pixel texture as fallback
        unsigned char pixels[] = {255, 255, 255, 255}; // RGBA white pixel
        if (!dummyTexture.loadFromMemory(pixels, sizeof(pixels))) {
            std::cerr << "Failed to create fallback texture\n";
        }
        currentSprite.setTexture(dummyTexture, true);
    }
    
    // Initialize buttons
    nextButton = std::make_unique<Button>(
        sf::Vector2f(120, 50),
        sf::Vector2f(650, 530),
        sf::Color(0, 150, 200),
        font,
        "Next"
    );
    
    prevButton = std::make_unique<Button>(
        sf::Vector2f(120, 50),
        sf::Vector2f(30, 530),
        sf::Color(200, 100, 0),
        font,
        "Back"
    );

    saveButton = std::make_unique<Button>(
        sf::Vector2f(200, 60), // Bigger button
        sf::Vector2f(350, 300), // Center of screen
        sf::Color(150, 0, 200), 
        font,
        "Save Game"
    );

    menuButton = std::make_unique<Button>(
        sf::Vector2f(150, 50),
        sf::Vector2f(325, 530),
        sf::Color::Green,
        font,
        "Menu"
    ); //goes back to start state
    
    updateButtons();
}

void ACT1_state::loadSprites() {
    auto& resMgr = ResourceManager::getInstance();
    for (int i = 1; i <= 11; i++) {
        std::string path = "assets/ACT1/ACT1_" + std::to_string(i) + ".png";
        try {
            sf::Texture texture = resMgr.getTexture(path);
            if (texture.getSize().x == 0 || texture.getSize().y == 0) {
                std::cerr << "Warning: Texture " << path << " has zero size\n";
                continue;
            }
            sprites.push_back(texture);
            std::cout << "Loaded texture: " << path << " (" 
                      << texture.getSize().x << "x" << texture.getSize().y << ")\n";
        } catch (const std::exception& e) {
            std::cerr << "Failed to load texture " << path << ": " << e.what() << "\n";
        }
    }
}

void ACT1_state::updateButtons() {
    nextButton->setVisible(currentIndex < sprites.size() - 1);
    prevButton->setVisible(currentIndex > 0);
    menuButton->setVisible(true);
    saveButton->setVisible(currentIndex == 0);  // Only show on first sprite
}

void ACT1_state::handleEvents(Game& game) {
    while (auto event = game.getWindow().pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            game.getWindow().close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            auto mousePos = game.getWindow().mapPixelToCoords(
                event->getIf<sf::Event::MouseButtonPressed>()->position);

            if (saveButton->isVisible() && saveButton->isClicked(mousePos)) {
               game.saveGame(1, currentIndex + 1);  // Call the save function
               AudioManager::getInstance().playSound("sparkle");
               
               // Set save feedback
               justSaved = true;
               saveMessageTimer = 3.0f; // Show message for 3 seconds
               continue;  
            }

            if (nextButton->isVisible() && nextButton->isClicked(mousePos)) {
                if (currentIndex < sprites.size() - 1) {
                    currentIndex++;
                    currentSprite.setTexture(sprites[currentIndex], true); // Reset rect
                    updateButtons();
                    AudioManager::getInstance().playSound("click");
                    std::cout << "Advanced to sprite " << currentIndex + 1 
                              << " (size: " << sprites[currentIndex].getSize().x 
                              << "x" << sprites[currentIndex].getSize().y << ")\n";
                }
            }
            else if (prevButton->isVisible() && prevButton->isClicked(mousePos)) {
                if (currentIndex > 0) {
                    currentIndex--;
                    currentSprite.setTexture(sprites[currentIndex], true); // Reset rect
                    updateButtons();
                    AudioManager::getInstance().playSound("click");
                    std::cout << "Back to sprite " << currentIndex + 1 
                              << " (size: " << sprites[currentIndex].getSize().x 
                              << "x" << sprites[currentIndex].getSize().y << ")\n";
                }
            }
            else if (menuButton->isClicked(mousePos)) {
                game.changeState(std::make_unique<StartState>(game.getFont()));
                AudioManager::getInstance().playSound("click");
            }
        }
    }
}

void ACT1_state::update(Game& game) {
    // Update save message timer
    if (saveMessageTimer > 0.0f) {
        saveMessageTimer -= 0.016f; // Assuming ~60 FPS (16ms per frame)
        if (saveMessageTimer <= 0.0f) {
            justSaved = false;
        }
    }
}

void ACT1_state::render(Game& game) {
    game.getWindow().clear();
    
    // Always try to draw the sprite (we know we have valid textures)
    sf::Vector2u windowSize = game.getWindow().getSize();
    sf::Vector2u textureSize = currentSprite.getTexture().getSize();
    
    // Reset scale and position each frame
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::min(scaleX, scaleY);
    currentSprite.setScale(sf::Vector2f(scale, scale));
    
    // Center sprite
    currentSprite.setPosition(sf::Vector2f(
        windowSize.x/2.f, 
        windowSize.y/2.f
    ));
    
    currentSprite.setOrigin(sf::Vector2f(
        textureSize.x/2.f, 
        textureSize.y/2.f
    ));
    
    game.getWindow().draw(currentSprite);
    
    // Draw buttons
    if (nextButton->isVisible()) nextButton->draw(game.getWindow());
    if (prevButton->isVisible()) prevButton->draw(game.getWindow());
    
    // Show save feedback on first sprite only
    if (saveButton->isVisible()) {
        saveButton->draw(game.getWindow());
        
        if (justSaved && saveMessageTimer > 0.0f) {
            // Show "Game Saved!" message
            sf::Text saveConfirmation(font);
            saveConfirmation.setString("Game Saved!");
            saveConfirmation.setCharacterSize(28);
            saveConfirmation.setFillColor(sf::Color::Green);
            saveConfirmation.setStyle(sf::Text::Bold);
            
            // Add outline for better visibility on white background
            saveConfirmation.setOutlineThickness(2.0f);
            saveConfirmation.setOutlineColor(sf::Color::Black);
            
            // Center the text
            sf::FloatRect textBounds = saveConfirmation.getLocalBounds();
            saveConfirmation.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x/2.0f,
                                     textBounds.position.y + textBounds.size.y/2.0f));
            saveConfirmation.setPosition(sf::Vector2f(400, 200)); // Above save button
            
            game.getWindow().draw(saveConfirmation);
        }
        else if (game.hasSavedGame()) {
            // Show last save time
            sf::Text saveStatus(font);
            saveStatus.setString("Last saved: " + game.getSaveTime());
            saveStatus.setCharacterSize(18);
            saveStatus.setFillColor(sf::Color::Black); // Black text for white background
            saveStatus.setPosition(sf::Vector2f(250, 370));
            game.getWindow().draw(saveStatus);
        }
    }

    menuButton->draw(game.getWindow());
    game.getWindow().display();
}