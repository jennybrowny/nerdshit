#include <fstream> 
#include "game.hpp" 
#include <iostream>
#include "states/start_state.hpp"
#include "states/credits_state.hpp"
#include "states/tutorial_state.hpp"

StartState::StartState(sf::Font& font) {
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
        // Save button (centered below start button)
        saveButton = std::make_unique<Button>(
            sf::Vector2f(200, 80),
            sf::Vector2f(300, 350),
            sf::Color::Blue, 
            font,
            "SAVED"
        );

        // credits button 
        creditsButton = std::make_unique<Button>(
            sf::Vector2f(200, 80),
            sf::Vector2f(300, 440),
            sf::Color::Transparent,
            font,
            "CREDITS"
        );
        creditsButton->setOutlineEnabled(false);
        creditsButton->setHoverColor(sf::Color::Yellow);
        std::cout << "Menu buttons created successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ERROR: Could not create buttons - " << e.what() << std::endl;
    } 
    std::cout << "StartState constructor completed." << std::endl;
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
            else if (saveButton && saveButton->isClicked(mousePos)) {
                std::cout << "SAVE button clicked!" << std::endl;
                AudioManager::getInstance().playSound("click");
                // Handle save button click
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
    if (saveButton) saveButton->update(mousePos);
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

    // Draw buttons
    if (startButton) {
        std::cout << "Drawing start button" << std::endl;
        startButton->draw(game.getWindow());
    } 
    if (saveButton) { 
        std::cout << "Drawing save button" << std::endl;
        saveButton->draw(game.getWindow());
    }
    if (creditsButton) { 
        std::cout << "Drawing credits button" << std::endl;
        creditsButton->draw(game.getWindow());
    }

    game.getWindow().display();
    std::cout << "StartState::render completed" << std::endl;
}