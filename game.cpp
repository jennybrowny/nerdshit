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
    window(sf::VideoMode({800, 600}), "NerdShit"),  // Main game window
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
        "assets/ACT0/ACT01_1.PNG",
        "assets/ACT0/ACT01_2.PNG", 
        "assets/ACT0/ACT01_3.PNG"
    };

    // Load each image file into textures
    for (const auto& path : tutorialPaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            tutorialTextures.push_back(texture);
        } else {
            std::cerr << "ERROR: Failed to load tutorial image: " << path << std::endl;
        }
    }

    // SFML 3.0: Initialize currentTutorialSprite with first texture if available
    if (!tutorialTextures.empty()) {
        currentTutorialSprite = std::make_unique<sf::Sprite>(tutorialTextures[0]);
    }

    loadResources();    // Load fonts, audio, etc.
    createButtons();    // Initialize UI buttons
}

// -------------------------------------------------------------------
// Destructor
// -------------------------------------------------------------------
Game::~Game() {
    // Smart pointers automatically clean up
}

// -------------------------------------------------------------------
// loadResources(): Loads all external assets
// -------------------------------------------------------------------
void Game::loadResources() {
    auto& audio = AudioManager::getInstance(); // Singleton Pattern

    // Audio loading
    audio.loadMusic("background", "assets/background/Intro.mp3");
    audio.loadSound("click", "assets/sounds/button_click.mp3");

    // Debug output for loading phase
    std::cout << "================================\n";
    std::cout << "||        LOADING ASSETS      ||\n";
    std::cout << "================================\n";

    // Try to load a font, but don't crash if none available
    fontLoaded = false;
    std::vector<std::string> fontPaths = {
        "arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Helvetica.ttc"  // macOS system font
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

    // Title screen setup
    std::cout << "Attempting to load title screen texture..." << std::endl;
    if (titleTexture.loadFromFile("assets/background/title_screen.png")) {
        std::cout << "Title texture loaded successfully!" << std::endl;
        titleSprite = std::make_unique<sf::Sprite>(titleTexture);
        scaleSpriteToWindow(*titleSprite);
        std::cout << "Title sprite created and scaled" << std::endl;
    } else {
        std::cout << "WARNING: Failed to load title screen texture - using fallback color" << std::endl;
        bgColor = sf::Color(50, 100, 150);  // Fallback color
    }
}

// -------------------------------------------------------------------
// createButtons(): Initializes all UI buttons
// -------------------------------------------------------------------
void Game::createButtons() {
    // Main start button (only create if font loaded)
    if (fontLoaded) {
        startButton = std::make_unique<Button>(
            sf::Vector2f(200, 80),        // Size
            sf::Vector2f(300, 260),       // Position (centered)
            sf::Color::Green,            // Color
            font,                        // Font
            "START"                      // Label
        );

        // Navigation buttons
        nextButton = std::make_unique<Button>(
            sf::Vector2f(120, 50),
            sf::Vector2f(650, 530),       // Bottom right
            sf::Color(0, 150, 200),      // Blue
            font,
            "Next"
        );

        prevButton = std::make_unique<Button>(
            sf::Vector2f(120, 50),
            sf::Vector2f(30, 530),        // Bottom left
            sf::Color(200, 100, 0),      // Orange
            font,
            "Back"
        );
    } else {
        std::cout << "Warning: Buttons will not display properly without fonts" << std::endl;
    }

    // Page counter configuration (only if font loaded)
    if (fontLoaded) {
        pageText.setFont(font);
        pageText.setCharacterSize(24);
        pageText.setFillColor(sf::Color::White);
        pageText.setPosition(sf::Vector2f(400, 550));
    }
}

/* 
 * =============================================
 * GAME LOOP FUNCTIONS
 * =============================================
 */

// -------------------------------------------------------------------
// run(): Main game loop
// -------------------------------------------------------------------
void Game::run() {
    std::cout << "=== STARTING GAME LOOP ===" << std::endl;
    std::cout << "Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;

    if (fontLoaded) {
        std::cout << "Creating StartState with loaded font..." << std::endl;
        currentState = std::make_unique<StartState>(font);
    } else {
        // Create a minimal start state without font-dependent features
        std::cout << "Running in font-less mode" << std::endl;
        std::cout << "Creating StartState without proper font..." << std::endl;
        currentState = std::make_unique<StartState>(font); // Still pass font, but StartState will handle it gracefully
    }

    std::cout << "StartState created successfully: " << (currentState ? "YES" : "NO") << std::endl;
    std::cout << "Window is open: " << (window.isOpen() ? "YES" : "NO") << std::endl;
    std::cout << "Window size: " << window.getSize().x << "x" << window.getSize().y << std::endl;
    std::cout << "Entering main game loop..." << std::endl;

    while (window.isOpen()) {
        currentState->handleEvents(*this);
        currentState->update(*this);
        currentState->render(*this);
    }

    std::cout << "Game loop ended" << std::endl;
}

// -------------------------------------------------------------------
// handleEvents(): Processes input events
// -------------------------------------------------------------------
void Game::handleEvents() {
    // Delegated to current state
}

// -------------------------------------------------------------------
// update(): Handles frame-by-frame game logic
// -------------------------------------------------------------------
void Game::update() {
    // Music fading effect when not on start screen
    auto& audio = AudioManager::getInstance();
    float vol = audio.getMusicVolume();
    if (vol > 5.f) {
        audio.setMusicVolume(vol - 0.5f);  // Gradual volume reduction
    } else {
        audio.stopMusic();
        isMusicPlaying = false;
    }
}

// -------------------------------------------------------------------
// render(): Draws everything to screen
// -------------------------------------------------------------------
void Game::render() {
    // Delegated to current state
}

/* 
 * =============================================
 * UTILITY FUNCTIONS
 * =============================================
 */

// -------------------------------------------------------------------
// scaleSpriteToWindow(): Fits sprites to window size
// -------------------------------------------------------------------
void Game::scaleSpriteToWindow(sf::Sprite& sprite) {
    auto bounds = sprite.getGlobalBounds();
    sprite.setScale(sf::Vector2f(
        window.getSize().x / bounds.size.x, // Scale to fit width
        window.getSize().y / bounds.size.y  // Scale to fit height
    ));
}

// -------------------------------------------------------------------
// updatePageIndicator(): Updates page counter text
// -------------------------------------------------------------------
void Game::updatePageIndicator() {
    std::stringstream ss;
    ss << (currentTutorialIndex + 1) << "/" << tutorialTextures.size();
    pageText.setString(ss.str());
}

// -------------------------------------------------------------------
// goToNextPage(): Advances to the next tutorial slide
// -------------------------------------------------------------------
void Game::goToNextPage() {
    if (!tutorialTextures.empty() && currentTutorialSprite) {
        currentTutorialIndex = (currentTutorialIndex + 1) % tutorialTextures.size();
        currentTutorialSprite->setTexture(tutorialTextures[currentTutorialIndex]);
        updatePageIndicator();
    }
}

// -------------------------------------------------------------------
// goToPreviousPage(): Goes back to previous tutorial slide
// -------------------------------------------------------------------
void Game::goToPreviousPage() {
    if (!tutorialTextures.empty() && currentTutorialSprite) {
        currentTutorialIndex = (currentTutorialIndex - 1 + tutorialTextures.size()) % tutorialTextures.size();
        currentTutorialSprite->setTexture(tutorialTextures[currentTutorialIndex]);
        updatePageIndicator();
    }
}

// -------------------------------------------------------------------
// changeState(): Changes the current game state
// -------------------------------------------------------------------
void Game::changeState(std::unique_ptr<GameState> newState) {
    currentState = std::move(newState);
}