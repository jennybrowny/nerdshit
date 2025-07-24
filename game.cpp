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
    isMusicPlaying(false),
// tutorial (pretty much just three sprites)
    currentTutorialSprite()  // Explicitly initialize the sprite
    currentTutorialIndex(0)  // since I have 3 pages of sprite we need to count them

{
    // Load tutorial images
    std::vector<std::string> tutorialImagePaths = {
        "assets/ACT0/ACT01_1.png", 
       // "assets/ACT0/ACT01_2.png",
   //    "assets/ACT0/ACT01_3.png"
    };
    
    for (const auto& path : tutorialImagePaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            tutorialTextures.push_back(texture);
        } else {
            std::cout << "Failed to load tutorial image: " << path << std::endl;
        }
    }
    
    if (!tutorialTextures.empty()) {
        currentTutorialSprite.setTexture(tutorialTextures[0]);
        sf::FloatRect bounds = currentTutorialSprite.getLocalBounds();
        currentTutorialSprite.setScale(
            (float)window.getSize().x / bounds.width,
            (float)window.getSize().y / bounds.height
        );
    }

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

    // 3. Create button with text (now with automatic sound)
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
// show png as sprite ACT01_1
 if (!tutorialTextures.empty()) {
window.draw(currentTutorialSprite);  // <-- THIS SHOWS THE SPRITE
        }
                break;

            case QUIZ_SCREEN:
                break;
        }

        window.display();
    }
}