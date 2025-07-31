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
    currentTutorialSprite(tutorialTextures[0]), // Initialize with first texture
    bgColor(sf::Color::Green), // Fixed typo (was bgColorsf)
    currentState(nullptr),                   // Initial game state 
    isMusicPlaying(false),                         // Audio state flag
    currentTutorialIndex(0),                       // Tutorial page index
    titleSprite(nullptr),                          // Title screen image
    nextButton(nullptr),                           // TBD
    prevButton(nullptr),                          // TBD
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

    currentTutorialSprite,  // Default construct first
    bgColorsf::Color::Green, // Default background color

    loadResources();    // Load fonts, audio, etc.
    createButtons();    // Initialize UI buttons
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

    // Font loading with fallback system
if (!font.openFromFile("assets/fonts/arial.ttf")) {
    std::cerr << "Main font failed, trying system fallback...\n";
    if (!font.openFromFile("/System/Library/Fonts/Arial.ttf")) {
        throw std::runtime_error("Critical: No fonts available");
    }
}
    // Title screen setup
    if (titleTexture.loadFromFile("assets/background/title_screen.png")) {
        titleSprite = std::make_unique<sf::Sprite>(titleTexture);
        scaleSpriteToWindow(*titleSprite);
    } else {
        bgColor = sf::Color(50, 100, 150);  // Fallback color
    }
}

// -------------------------------------------------------------------
// createButtons(): Initializes all UI buttons
// -------------------------------------------------------------------
void Game::createButtons() {
    // Main start button
    startButton = std::make_unique<Button>(
        sf::Vector2f(200, 80),        // Size
        sf::Vector2f(300, 260),       // Position (centered)
        sf::Color::Yellow,            // Color
        font,                        // Font
        "START"                      // Label
    );

    // Navigation buttons
    nextButton = std::make_unique<Button>(
        sf::Vector2f(120, 50),
        sf::Vector2f(650, 530),       // Bottom right
        sf::Color(0, 150, 200),      // Blue
        font,
        "Next →"
    );

    prevButton = std::make_unique<Button>(
        sf::Vector2f(120, 50),
        sf::Vector2f(30, 530),        // Bottom left
        sf::Color(200, 100, 0),      // Orange
        font,
        "← Back"
    );

    // Page counter configuration
    pageText.setFont(font);
    pageText.setCharacterSize(24);
    pageText.setFillColor(sf::Color::White);
    pageText.setPosition(sf::Vector2f(400, 550));

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
    currentState = std::make_unique<StartState>(font); // Single initialization
    
    while (window.isOpen()) {
        currentState->handleEvents(*this);
        currentState->update(*this);
        currentState->render(*this);
    }
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
    sprite.setScale(
        window.getSize().x / bounds.width,
        window.getSize().y / bounds.height
    );
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
void Game::goToPreviousPage() {
    if (!tutorialTextures.empty()) {
        currentTutorialIndex = (currentTutorialIndex - 1 + tutorialTextures.size()) % tutorialTextures.size();
    }
}
// -------------------------------------------------------------------
// changeState(): Changes the current game state
// -------------------------------------------------------------------
// This allows switching between different game states like Start, Tutorial, etc.
void Game::changeState(std::unique_ptr<GameState> newState) {
    currentState = std::move(newState);
}