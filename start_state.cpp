
#include "states/start_state.hpp"
#include "game.hpp" 
#include "states/tutorial_state.hpp"
#include <iostream>

StartState::StartState(sf::Font& font) {
    std::cout << "=== StartState Constructor Called ===" << std::endl;

    // Only create button if font is valid
    try {
        std::cout << "Attempting to create start button..." << std::endl;
        startButton = std::make_unique<Button>(
            sf::Vector2f(200, 80),
            sf::Vector2f(300, 260),
            sf::Color::Yellow,
            font,
            "START"
        );
        std::cout << "Start button created successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ERROR: Could not create start button - " << e.what() << std::endl;
    } catch (...) {
        std::cout << "ERROR: Unknown error creating start button" << std::endl;
    }

    std::cout << "StartState constructor completed. Button exists: " << (startButton ? "YES" : "NO") << std::endl;
}

void StartState::handleEvents(Game& game) {
    std::cout << "StartState::handleEvents called" << std::endl;
    
    // SFML 3.0 event handling
    while (auto event = game.getWindow().pollEvent()) {
        std::cout << "Processing event..." << std::endl;
        
        if (event->is<sf::Event::Closed>()) {
            std::cout << "Close event detected - closing window" << std::endl;
            game.getWindow().close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            std::cout << "Mouse button pressed" << std::endl;
            const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            auto mousePos = game.getWindow().mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});

            if (startButton && startButton->isClicked(mousePos)) {
                std::cout << "START button clicked!" << std::endl;
                AudioManager::getInstance().playSound("click");
                game.changeState(std::make_unique<TutorialState>(game));
            }
        }
        else if (event->is<sf::Event::KeyPressed>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                std::cout << "Escape key pressed - closing window" << std::endl;
                game.getWindow().close();
            }
        }
    }
}

void StartState::update(Game& game) {
    // Empty for now - could add animations here later
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
        // Fallback: draw a simple colored background
        sf::RectangleShape background(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color(50, 100, 150));
        game.getWindow().draw(background);
    }

    // Draw start button if available
    if (startButton) {
        std::cout << "Drawing start button" << std::endl;
        startButton->draw(game.getWindow());
    } else {
        std::cout << "No start button to draw" << std::endl;
    }

    // Display everything
    game.getWindow().display();
    std::cout << "StartState::render completed" << std::endl;
}
