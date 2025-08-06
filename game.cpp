#include "game.hpp" // headers
#include "button.hpp" // Buttons
#include <sstream> // For page number text
#include <iostream>
#include "states/start_state.hpp"
#include "states/tutorial_state.hpp"
#include "resource_manager.hpp"

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
    fontLoaded(false)                              // Font loading state
{
    // Load tutorial images for ACT0
    std::vector<std::string> tutorialPaths = {
        "assets/ACT0/ACT01_0.PNG", // first image
        "assets/ACT0/ACT01_1.PNG",
        "assets/ACT0/ACT01_2.PNG", 
        "assets/ACT0/ACT01_3.PNG" // last image
    };

    // Get resource manager instance
    auto& resMgr = ResourceManager::getInstance();

    // Load all tutorial textures through ResourceManager
    for (const auto& path : tutorialPaths) {
        tutorialTextures.push_back(resMgr.getTexture(path));
    }

    // Create sprite with first texture if available
    if (!currentTutorialSprite && !tutorialTextures.empty()) {
        currentTutorialSprite = std::make_unique<sf::Sprite>(tutorialTextures[0]);
        scaleSpriteToWindow(*currentTutorialSprite);
    }

    // Load other resources (fonts, audio, etc.)
    loadResources();
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
    fontLoaded = false;
    std::vector<std::string> fontPaths = {
        "arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Helvetica.ttc"
    };

auto& resMgr = ResourceManager::getInstance();
for (const auto& path : fontPaths) {
    try {
        font = resMgr.getFont(path);
        std::cout << "Successfully loaded font: " << path << std::endl;
        fontLoaded = true;
        break;
    } catch (const std::exception& e) {
        std::cout << "Failed to load font: " << path << " - " << e.what() << std::endl;
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
// run the game loop 
// -------------------------------------------------------------------
// Main game loop that handles events, updates state, and renders graphics
// -------------------------------------------------------------------
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
    window.clear();
    window.display();
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

