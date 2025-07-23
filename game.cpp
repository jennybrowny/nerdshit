#include "game.hpp"
#include "button.hpp"
#include <iostream>

Game::Game() : 
    window(sf::VideoMode(sf::Vector2u(800, 600)), "NerdShit"),
    currentTutorialSprite(),
    tutorialSound(),
    clickSound(),
    titleTexture(),
    titleSprite(nullptr),
    startButton(nullptr),
    bgColor(sf::Color::Black),
    currentScreen(START_SCREEN),
    isMusicPlaying(false),
    currentTutorialIndex(0)
{
    // Initialize sound buffer
    if (!clickBuffer.loadFromFile("assets/sounds/button_click.mp3")) {
        std::cout << "ERROR: Couldn't find assets/sounds/button_click.mp3\n";
    }
    clickSound.setBuffer(clickBuffer);

    // Load music
    if (!introMusic.openFromFile("assets/background/Intro.mp3")) {
        std::cout << "Failed to load intro music!" << std::endl;
    }

    // Load font
    std::cout << "=== loading font ===" << std::endl;
    if (!font.loadFromFile("/System/Library/Fonts/Arial.ttf")) {
        std::cout << "Failed to load font, trying backup..." << std::endl;
        if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cout << "Failed to load backup font too!" << std::endl;
        }
    }

    // Load ACT0 tutorial images
    std::vector<std::string> imagePaths = {
        "assets/ACT0/ACT01_1.png",
        "assets/ACT0/ACT01_2.png",
        "assets/ACT0/ACT01_3.png"
    };

    for (const auto& path : imagePaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            tutorialTextures.push_back(texture);
        } else {
            std::cout << "Failed to load tutorial image: " << path << std::endl;
        }
    }

    // Set first image (if loaded)
    if (!tutorialTextures.empty()) {
        currentTutorialSprite.setTexture(tutorialTextures[0]);
        sf::FloatRect bounds = currentTutorialSprite.getLocalBounds();
        currentTutorialSprite.setScale(
            static_cast<float>(window.getSize().x) / bounds.width,
            static_cast<float>(window.getSize().y) / bounds.height
        );
    }

    // Load and play looping sound
    if (!tutorialBuffer.loadFromFile("sounds/tut.mp3")) {
        std::cout << "Failed to load tut.mp3" << std::endl;
    }
    tutorialSound.setBuffer(tutorialBuffer);
    tutorialSound.setLoop(true);
    tutorialSound.play();

    // Create button with text and sound
    startButton = new Button(
        sf::Vector2f(200, 80),
        sf::Vector2f(300, 260),
        sf::Color::Green,
        font,
        "START",
        clickBuffer
    );
    
    // Set button callback
    startButton->setOnClick([this]() {
        currentScreen = TUTORIAL_SCREEN;
        bgColor = sf::Color(100, 100, 150);
    });

    // Load background image
    std::cout << "=== loading background image ===" << std::endl;
    if (titleTexture.loadFromFile("assets/background/title_screen.png")) {
        std::cout << "Success: image loaded!" << std::endl;
        titleSprite = new sf::Sprite(titleTexture);
        
        // Scale to fit window
        sf::Vector2u textureSize = titleTexture.getSize();
        sf::Vector2f scaleFactors(
            static_cast<float>(window.getSize().x) / textureSize.x,
            static_cast<float>(window.getSize().y) / textureSize.y
        );
        titleSprite->setScale(scaleFactors);
    } else {
        std::cout << "Failed to load image, using solid color" << std::endl;
        bgColor = sf::Color(50, 100, 150);
    }
}

Game::~Game() {
    delete startButton;
    delete titleSprite;
    introMusic.stop();
}

void Game::run() {
    std::cout << "=== game starting ===" << std::endl;
    
    if (currentScreen == START_SCREEN) {
        introMusic.play();
        introMusic.setLoop(true);
        introMusic.setVolume(100.f);
        isMusicPlaying = true;
    }

    sf::Clock fadeClock;

    while (window.isOpen()) {
        std::optional<sf::Event> eventOpt;
        while ((eventOpt = window.pollEvent())) {
            sf::Event& event = *eventOpt;
            
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                
                if (currentScreen == START_SCREEN) {
                    startButton->isClicked(mousePos);
                }
                else if (currentScreen == TUTORIAL_SCREEN) {
                    // Move to next image
                    currentTutorialIndex++;
                    
                    if (currentTutorialIndex < tutorialTextures.size()) {
                        currentTutorialSprite.setTexture(tutorialTextures[currentTutorialIndex]);
                        sf::FloatRect bounds = currentTutorialSprite.getLocalBounds();
                        currentTutorialSprite.setScale(
                            static_cast<float>(window.getSize().x) / bounds.width,
                            static_cast<float>(window.getSize().y) / bounds.height
                        );
                    } else {
                        // End of tutorial - stop sound and move to next screen
                        tutorialSound.stop();
                        currentScreen = QUIZ_SCREEN;
                    }
                }
            }
        }
        
        if (currentScreen == START_SCREEN) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            startButton->update(mousePos);
        }

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
        
        window.clear(bgColor);
        
        switch (currentScreen) {
            case START_SCREEN:
                if (titleSprite) window.draw(*titleSprite);
                startButton->draw(window);
                break;
                
            case TUTORIAL_SCREEN:
                window.draw(currentTutorialSprite);
                break;
                
            case QUIZ_SCREEN:
                // Quiz screen implementation
                break;
        }
        
        window.display();
    }
}