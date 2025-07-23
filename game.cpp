#include "game.hpp"
#include "button.hpp"
#include <iostream>

Game::Game() : 
    window(sf::VideoMode({800, 600}), "NerdShit"),
    titleTexture(),
    titleSprite(nullptr),
    startButton(nullptr),
    bgColor(sf::Color::Black),
    currentScreen(START_SCREEN),
    isMusicPlaying(false)
{
    // 1. Load music first
    if (!introMusic.openFromFile("assets/background/Intro.mp3")) {
        std::cout << "Failed to load intro music!" << std::endl;
    }

    // 2. Load font
    std::cout << "=== loading font ===" << std::endl;
    if (!font.openFromFile("/System/Library/Fonts/Arial.ttf")) {
        std::cout << "Failed to load font, trying backup..." << std::endl;
        if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cout << "Failed to load backup font too!" << std::endl;
        }
    }
    
    // 3. Create button with text
    startButton = new Button(sf::Vector2f(200, 80), sf::Vector2f(300, 260), sf::Color::Green, font, "START");
    
    // 4. Load background image
    std::cout << "=== loading background image ===" << std::endl;
    if (!titleTexture.loadFromFile("assets/background/title_screen.png")) {
        std::cout << "Failed to load image, using solid color" << std::endl;
        bgColor = sf::Color(50, 100, 150);
    } else {
        std::cout << "Success: image loaded!" << std::endl;
        titleSprite = new sf::Sprite(titleTexture);
        
        // Scale to fit window (SFML 3.0 requires Vector2f)
        sf::Vector2u textureSize = titleTexture.getSize();
        sf::Vector2f scaleFactors(
            (float)window.getSize().x / textureSize.x,
            (float)window.getSize().y / textureSize.y
        );
        titleSprite->setScale(scaleFactors);
    }
}

Game::~Game() {
    delete startButton;
    delete titleSprite;
    introMusic.stop();  // Ensure music stops
}

void Game::run() {
    std::cout << "=== game starting ===" << std::endl;
    
    // Start music if on start screen
    if (currentScreen == START_SCREEN) {
        introMusic.play();
        introMusic.setLooping(true);
        introMusic.setVolume(100.f);
        isMusicPlaying = true;
    }

    sf::Clock fadeClock;  // For smooth fade timing
    
    while (window.isOpen()) {
        // Handle music transitions
        if (currentScreen != START_SCREEN && isMusicPlaying) {
            if (fadeClock.getElapsedTime().asMilliseconds() > 50) {
                float currentVolume = introMusic.getVolume();
                if (currentVolume > 5.f) {
                    introMusic.setVolume(currentVolume - 5.f);
                } else {
                    introMusic.stop();
                    isMusicPlaying = false;
                }
                fadeClock.restart();
            }
        }
        
        // Event handling
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mousePos = sf::Mouse::getPosition(window);
                if (currentScreen == START_SCREEN && startButton->isClicked(sf::Vector2f(mousePos))) {
                    currentScreen = TUTORIAL_SCREEN;
                    bgColor = sf::Color(100, 100, 150);
                }
            }
        }
        
        // Rendering
        window.clear(bgColor);
        
        switch (currentScreen) {
            case START_SCREEN:
                if (titleSprite) window.draw(*titleSprite);
                startButton->draw(window);
                break;
                
            case TUTORIAL_SCREEN:
                // Tutorial content goes here
                break;
                
            case QUIZ_SCREEN:
                break;
        }
        
        window.display();
    }
}