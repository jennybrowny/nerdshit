#include <fstream> 
#include "game.hpp" 
#include <iostream>
#include <filesystem>
#include <sstream>
#include "states/start_state.hpp"
#include "states/credits_state.hpp"
#include "states/tutorial_state.hpp"
#include "states/ACT1_state.hpp"
#include "states/save_state.hpp"
#include "audio_manager.hpp"

StartState::StartState(sf::Font& font) : selectedSaveSlot(-1) {
    std::cout << "=== StartState Constructor Called ===" << std::endl;

    try {
        // Main action buttons - centered and spaced nicely
        startButton = std::make_unique<Button>(
            sf::Vector2f(250, 60),
            sf::Vector2f(275, 250),
            sf::Color(50, 150, 50), // Forest green
            font,
            "NEW GAME"
        );
        
        loadGameButton = std::make_unique<Button>(
            sf::Vector2f(250, 50),
            sf::Vector2f(275, 320),
            sf::Color(80, 120, 200), // Blue
            font,
            "LOAD GAME"
        );
        
        creditsButton = std::make_unique<Button>(
            sf::Vector2f(250, 50),
            sf::Vector2f(275, 380),
            sf::Color(100, 50, 150), // Purple
            font,
            "CREDITS"
        );
        
        exitButton = std::make_unique<Button>(
            sf::Vector2f(250, 50),
            sf::Vector2f(275, 440),
            sf::Color(150, 50, 50),  // Red color
            font,
            "EXIT GAME"
        );
        
        std::cout << "Menu buttons created successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ERROR: Could not create buttons - " << e.what() << std::endl;
    } 
    
    std::cout << "StartState constructor completed." << std::endl;
}

void StartState::handleEvents(Game& game) {
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow()));

    while (auto event = game.getWindow().pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            game.getWindow().close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            mousePos = game.getWindow().mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});

            if (startButton && startButton->isClicked(mousePos)) {
                game.changeState(std::make_unique<TutorialState>(game));
            }
            else if (loadGameButton && loadGameButton->isClicked(mousePos)) {
                game.changeState(std::make_unique<SaveState>(game.getFont()));
            }
            else if (creditsButton && creditsButton->isClicked(mousePos)) {
                game.changeState(std::make_unique<CreditsState>(game.getFont()));
            }
            else if (exitButton && exitButton->isClicked(mousePos)) {
                game.getWindow().close();
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
    if (loadGameButton) loadGameButton->update(mousePos);
    if (creditsButton) creditsButton->update(mousePos);
    if (exitButton) exitButton->update(mousePos);
}

void StartState::render(Game& game) {
    // Gradient background
    game.getWindow().clear(sf::Color(20, 25, 40));
    
    // Draw gradient background effect
    sf::RectangleShape gradientTop(sf::Vector2f(800, 300));
    gradientTop.setPosition(sf::Vector2f(0, 0));
    gradientTop.setFillColor(sf::Color(30, 40, 60, 150));
    game.getWindow().draw(gradientTop);
    
    // Draw title sprite if available - scaled to full window
    if (game.getTitleSprite()) {
        game.scaleSpriteToWindow(*game.getTitleSprite());
        game.getWindow().draw(*game.getTitleSprite());
    } else {
        // Fallback title text
        sf::Text titleText(game.getFont());
        titleText.setString("NERDSHIT GAME");
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(sf::Vector2f(titleBounds.position.x + titleBounds.size.x/2.0f,
                           titleBounds.position.y + titleBounds.size.y/2.0f));
        titleText.setPosition(sf::Vector2f(400, 100));
        game.getWindow().draw(titleText);
    }

    // Draw main buttons
    if (startButton) startButton->draw(game.getWindow());
    if (loadGameButton) loadGameButton->draw(game.getWindow());
    if (creditsButton) creditsButton->draw(game.getWindow());
    if (exitButton) exitButton->draw(game.getWindow());
    
    // Version/info text
    sf::Text versionText(game.getFont());
    versionText.setString("Press ESC to quit");
    versionText.setCharacterSize(12);
    versionText.setFillColor(sf::Color(100, 100, 100));
    versionText.setPosition(sf::Vector2f(10, 580));
    game.getWindow().draw(versionText);

    // Instructions
    sf::Text instructionText(game.getFont());
    instructionText.setString("Click 'Load Game' to manage your saved games");
    instructionText.setCharacterSize(14);
    instructionText.setFillColor(sf::Color(180, 180, 100));
    instructionText.setPosition(sf::Vector2f(275, 500));
    game.getWindow().draw(instructionText);

    game.getWindow().display();
}