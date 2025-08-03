#include "game.hpp" // headers
#include "button.hpp" // Buttons
#include <sstream> // For page number text
#include <iostream>
#include "states/start_state.hpp"
#include "states/tutorial_state.hpp"

/* 
███╗   ██╗███████╗██████╗ ██████╗ ███████╗██╗  ██╗██╗████████╗
████╗  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██║  ██║██║╚══██╔══╝
██╔██╗ ██║█████╗  ██████╔╝██║  ██║███████╗███████║██║   ██║   
██║╚██╗██║██╔══╝  ██╔══██╗██║  ██║╚════██║██╔══██║██║   ██║   
██║ ╚████║███████╗██║  ██║██████╔╝███████║██║  ██║██║   ██║  Jenny Brown 
╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝   ╚═╝  ECE205, Summer 2025 
*/

/* 
 * =============================================
 * GAME CLASS IMPLEMENTATION 
 * =============================================
 */

// -------------------------------------------------------------------
// Constructor: Initializes all game systems and resources
// -------------------------------------------------------------------
Game::Game() : 
    window(sf::VideoMode({800, 600}), "NerdShit"),  // Main game window. 800x600 pixel size for sprite scaling
    pageText(font, "", 24),                         // Page counter text
    bgColor(sf::Color::Green),                      // Background fallback color
    currentState(nullptr),                          // Initial game state 
    isMusicPlaying(false),                         // Audio state flag
    currentTutorialIndex(0),                       // Tutorial page index
    titleSprite(nullptr),                          // Title screen image
    nextButton(nullptr),                           // TBD
    prevButton(nullptr),                           // TBD
    fontLoaded(false)                              // Font loading state
{
    // Load tutorial images for ACT0
    std::vector<std::string> tutorialPaths = {
        "assets/ACT0/ACT01_0.PNG", // first image
        "assets/ACT0/ACT01_1.PNG",
        "assets/ACT0/ACT01_2.PNG", 
        "assets/ACT0/ACT01_3.PNG" // last image
    };

    // Load each image file into textures
    for (const auto& path : tutorialPaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            tutorialTextures.push_back(texture);
            
            // Create sprite with first texture
            if (!currentTutorialSprite && !tutorialTextures.empty()) {
                currentTutorialSprite = std::make_unique<sf::Sprite>(tutorialTextures[0]);
                scaleSpriteToWindow(*currentTutorialSprite);
            }
        }
    }

    loadResources();
    createButtons();
}

Game::~Game() {
    // Smart pointers automatically clean up
}

void Game::loadResources() {
    auto& audio = AudioManager::getInstance();

    // Audio loading music for background and sound effects
    audio.loadMusic("background", "assets/sounds/Intro.mp3");
    audio.loadMusic("act0", "assets/sounds/ACT0.mp3");
    audio.loadSound("click", "assets/sounds/button_click.mp3");

    std::cout << "================================\n";
    std::cout << "||        LOADING ASSETS      ||\n";
    std::cout << "================================\n";

    fontLoaded = false;
    std::vector<std::string> fontPaths = {
        "arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Helvetica.ttc"
    };

    for (const auto& path : fontPaths) {
        if (font.openFromFile(path)) {
            std::cout << "Successfully loaded font: " << path << std::endl;
            fontLoaded = true;
            break;
        }
    }

    if (!fontLoaded) {
        std::cout << "Warning: No fonts loaded - text will not display properly" << std::endl;
    }

    std::cout << "Attempting to load title screen texture..." << std::endl;
    if (titleTexture.loadFromFile("assets/background/title_screen.png")) {
        std::cout << "Title texture loaded successfully!" << std::endl;
        titleSprite = std::make_unique<sf::Sprite>(titleTexture);
        scaleSpriteToWindow(*titleSprite);
        std::cout << "Title sprite created and scaled" << std::endl;
    } else {
        std::cout << "WARNING: Failed to load title screen texture - using fallback color" << std::endl;
        bgColor = sf::Color(50, 100, 150);
    }
}

void Game::createButtons() {
    if (fontLoaded) {
    startButton = std::make_unique<Button>(
    sf::Vector2f(200, 60),  // Slightly slimmer button
    sf::Vector2f(
        window.getSize().x / 2 - 100,  // Center horizontally (200 width / 2)
        400                            // Position vertically
    ),
    sf::Color(0, 150, 0),   // Dark green background
    font,
    "START"
);

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
    } else {
        std::cout << "Warning: Buttons will not display properly without fonts" << std::endl;
    }

    if (fontLoaded) {
        pageText.setFont(font);
        pageText.setCharacterSize(24);
        pageText.setFillColor(sf::Color::White);
        pageText.setPosition(sf::Vector2f(400, 550));
    }
}

void Game::run() {
    std::cout << "=== STARTING GAME LOOP ===" << std::endl;
    std::cout << "Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;
// intro music queues
    if (isMusicPlaying) {
        AudioManager::getInstance().playMusic("background");
    } else {
        AudioManager::getInstance().setMusicVolume(50.f);
        AudioManager::getInstance().playMusic("background");
        isMusicPlaying = true;
    }
    // Initialize the first state 
    if (fontLoaded) {
        currentState = std::make_unique<StartState>(font);
    } else {
        currentState = std::make_unique<StartState>(font);
    }

    while (window.isOpen()) {
        currentState->handleEvents(*this);
        currentState->update(*this);
        currentState->render(*this);
    }
}

void Game::handleEvents() {
    // Delegated to current state
}

void Game::update() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    startButton->update(mousePos);  
    auto& audio = AudioManager::getInstance();
    float vol = audio.getMusicVolume();
    if (vol > 5.f) {
        audio.setMusicVolume(vol - 0.5f);
    } else {
        audio.stopMusic();
        isMusicPlaying = false;
    }
}

void Game::render() {
    // Delegated to current state
}

void Game::scaleSpriteToWindow(sf::Sprite& sprite) {
    const sf::Texture& texture = sprite.getTexture();
    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scale = std::min(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setOrigin(sf::Vector2f(
        textureSize.x / 2.0f,
        textureSize.y / 2.0f
    ));
    sprite.setPosition(sf::Vector2f(
        windowSize.x / 2.0f,
        windowSize.y / 2.0f
    ));
}

void Game::updatePageIndicator() {
    std::stringstream ss;
    ss << (currentTutorialIndex + 1) << "/" << tutorialTextures.size();
    pageText.setString(ss.str());
}

void Game::goToNextPage() {
    if (!tutorialTextures.empty()) {
        currentTutorialIndex = (currentTutorialIndex + 1) % tutorialTextures.size();
        currentTutorialSprite->setTexture(tutorialTextures[currentTutorialIndex]);
        scaleSpriteToWindow(*currentTutorialSprite);
        updatePageIndicator();
        AudioManager::getInstance().playSound("click"); // Add click sound
    }
}

void Game::goToPreviousPage() {
    if (!tutorialTextures.empty()) {
        currentTutorialIndex = (currentTutorialIndex - 1 + tutorialTextures.size()) % tutorialTextures.size();
        currentTutorialSprite->setTexture(tutorialTextures[currentTutorialIndex]);
        scaleSpriteToWindow(*currentTutorialSprite);
        updatePageIndicator();
        AudioManager::getInstance().playSound("click"); // Add click sound
    }
}

void Game::changeState(std::unique_ptr<GameState> newState) {
    // If transitioning to TutorialState, switch music to minecraft nerdshit
    if (dynamic_cast<TutorialState*>(newState.get())) {
        AudioManager::getInstance().playMusic("act0", true);
    }
    currentState = std::move(newState);
}