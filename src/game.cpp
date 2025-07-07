#include "/Users/jennybrown/nershit/include/game.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "/Users/jennybrown/nershit/include/button.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/BlendMode.hpp>

// Game constructor: initializes window and start button, then loads resources.
Game::Game() : 
    window_(sf::VideoMode({400, 300}), "Simple Start Screen"),
    startButton_({100.f, 50.f}, {150.f, 125.f}, 
                sf::Color(0, 100, 0), sf::Color::White, 2.f) {
    loadResources();
}

// Loads textures, fonts, and sets up the start button text.
void Game::loadResources() {
    if (backgroundLoaded_) {
        backgroundSprite_.setTexture(backgroundTexture_);
        sf::Vector2u textureSize = backgroundTexture_.getSize();
        backgroundSprite_.setScale(sf::Vector2f(400.f / textureSize.x, 300.f / textureSize.y));
    } else {
        std::cout << "Failed to load background image" << std::endl;
    }

    if (!font_.loadFromFile("resources/fonts/font.ttf")) {
        std::cout << "Failed to load font" << std::endl;
        std::exit(1);
    }

    startButton_.setText("START", font_, 24, sf::Color::White, sf::Text::Bold);
}

// Main game loop: processes events, updates state, and renders.
void Game::run() {
    while (window_.isOpen()) {
        processEvents();
        update();
        render();
    }
}

// Handles window and mouse events, including button clicks.
void Game::processEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::EventType::Closed)
            window_.close();

        if (event.type == sf::Event::EventType::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
            if (startButton_.isClicked(mousePos)) {
                std::cout << "Button clicked!" << std::endl;
                // Add your button click logic here
            }
        }
    }
}

// Renders the background and start button to the window.
void Game::render() {
    window_.clear(sf::Color::Black);
    if (backgroundLoaded_)
        window_.draw(backgroundSprite_);
    startButton_.draw(window_);
    window_.display();
}