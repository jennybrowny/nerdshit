

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
        sf::Vector2f(200, 80),    // Slightly smaller than start button
        sf::Vector2f(300, 350),   // Positioned below start button
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
        creditsButton->setOutlineEnabled(false); // Disable outline for this button only
        creditsButton->setHoverColor(sf::Color::Yellow);  // Make credits glow when hovered!
        std::cout << "Menu buttons created successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ERROR: Could not create nerdshit buttons - " << e.what() << std::endl;
    } 
    std::cout << "StartState constructor completed. Button exists: " << (startButton ? "YES" : "NO") << std::endl;
}

void StartState::handleEvents(Game& game) {
    std::cout << "StartState::handleEvents called" << std::endl;
        sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow()));
    if (saveButton->isClicked(mousePos)) {
        std::ofstream saveFile("save.dat");
        saveFile << "TUTORIAL_PAGE:" << game.getCurrentTutorialIndex();
        AudioManager::getInstance().playSound("click");
    }
   
if (creditsButton && creditsButton->isClicked(mousePos)) {
    std::cout << "CREDITS button clicked! Prepare for fame!" << std::endl;
    AudioManager::getInstance().playSound("click");
    game.changeState(std::make_unique<CreditsState>(game.getFont()));
}

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
        sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow())
    );
    startButton->update(mousePos); // Update hover state
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

    // Draw start, save, credit button on the menu 
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


    // Display everything
    game.getWindow().display();
    std::cout << "StartState::render completed" << std::endl;
}
