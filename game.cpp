#include "game.hpp" // headers
#include "button.hpp" // Buttons
#include <sstream> // For page number text
#include <iostream>


/* 
███╗   ██╗███████╗██████╗ ██████╗ ███████╗██╗  ██╗██╗████████╗
████╗  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██║  ██║██║╚══██╔══╝
██╔██╗ ██║█████╗  ██████╔╝██║  ██║███████╗███████║██║   ██║   
██║╚██╗██║██╔══╝  ██╔══██╗██║  ██║╚════██║██╔══██║██║   ██║   
██║ ╚████║███████╗██║  ██║██████╔╝███████║██║  ██║██║   ██║  Jenny Brown 
╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝   ╚═╝  ECE205, Summer 2025 
*/

#include "game.hpp"
#include "button.hpp"
#include <sstream>
#include <iostream>

/*
TO DO 7/24
okay so your game is fucking bloated
--
you need actual game states to bring these lines of code down
--
used sound manager as compostion NOT inhertiance:
No clear second pattern:

Claimed "State Pattern" is just an enum switch (not a true pattern)

SoundManager is pseudo-Singleton (static vars ≠ Singleton)
--
Button has dead code (IButtonBehavior isn't utilized)

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
    currentTutorialSprite(sf::Texture(), sf::IntRect())  // Initialize with empty texture
                                                         // Current displayed image
    bgColor(sf::Color::Green),                     // Fallback background
    currentScreen(START_SCREEN),                   // Initial game state
    isMusicPlaying(false),                         // Audio state flag
    currentTutorialIndex(0),                       // Tutorial page index
    titleSprite(nullptr),                          // Title screen image
    nextButton(nullptr),                           // TBD
    prevButton(nullptr)                            // TBD
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

    // Initialize first tutorial page if loaded
    if (!tutorialTextures.empty()) {
        currentTutorialSprite.setTexture(tutorialTextures[0]);
        scaleSpriteToWindow(currentTutorialSprite);
    }

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
    if (!font.openFromFile("assets/font/times.ttf")) {
        if (!font.openFromFile("/System/Library/Fonts/times.ttf")) {
            std::cerr << "CRITICAL: No fonts available!" << std::endl;
            static sf::Font dummyFont;  // Fallback to prevent crashes
            font = dummyFont;
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
    pageText.setPosition(400, 550);   // Bottom center
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
    // Start background music
    auto& audio = AudioManager::getInstance();
    audio.playMusic("background");
    audio.setMusicLoop(true);
    isMusicPlaying = true;

    // Core game loop
    while (window.isOpen()) {
        handleEvents();  // Input handling
        update();       // Game state updates
        render();       // Rendering
    }
}

// -------------------------------------------------------------------
// handleEvents(): Processes all input events (Observer)
// -------------------------------------------------------------------
void Game::handleEvents() {
    sf::Event event;
    auto& audio = AudioManager::getInstance();
    
    while (window.pollEvent(event)) {
        // Window close event
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Mouse click handling
        if (event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
            // Start screen button
            if (currentScreen == START_SCREEN && startButton->isClicked(mousePos)) {
                audio.playSound("click");
                currentScreen = TUTORIAL_SCREEN;  // Transition to tutorial
            }
            // Tutorial screen navigation
            else if (currentScreen == TUTORIAL_SCREEN) {
                if (nextButton->isClicked(mousePos)) {
                    audio.playSound("click");
                    goToNextPage();
                }
                else if (prevButton->isClicked(mousePos)) {
                    audio.playSound("click");
                    goToPreviousPage();
                }
            }
        }
    }
}

/* 
 * =============================================
 * GAME STATE MANAGEMENT 
 * =============================================
 */

// -------------------------------------------------------------------
// goToNextPage(): Advances tutorial pages
// -------------------------------------------------------------------
void Game::goToNextPage() {
    if (tutorialTextures.empty()) return;
    
    // Circular navigation through pages
    currentTutorialIndex = (currentTutorialIndex + 1) % tutorialTextures.size();
    currentTutorialSprite.setTexture(tutorialTextures[currentTutorialIndex]);
    updatePageIndicator();  // Update page counter display
}

// -------------------------------------------------------------------
// goToPreviousPage(): Returns to previous tutorial pages
// -------------------------------------------------------------------
void Game::goToPreviousPage() {
    if (tutorialTextures.empty()) return;
    
    // Circular navigation with underflow protection
    currentTutorialIndex = (currentTutorialIndex - 1 + tutorialTextures.size()) % tutorialTextures.size();
    currentTutorialSprite.setTexture(tutorialTextures[currentTutorialIndex]);
    updatePageIndicator();
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
 * RENDERING FUNCTIONS
 * =============================================
 */

// -------------------------------------------------------------------
// render(): Draws all game elements
// -------------------------------------------------------------------
void Game::render() {
    window.clear(bgColor);

    switch (currentScreen) {
        case START_SCREEN:
            if (titleSprite) window.draw(*titleSprite);
            startButton->draw(window);
            break;

        case TUTORIAL_SCREEN:
            window.draw(currentTutorialSprite);
            
            // Conditional button visibility
            if (currentTutorialIndex > 0) {
                prevButton->draw(window);
            }
            if (currentTutorialIndex < tutorialTextures.size() - 1) {
                nextButton->draw(window);
            }
            
            window.draw(pageText);  // Page counter
            break;

        case QUIZ_SCREEN:
            // Reserved for future implementation
            break;
    }

    window.display();
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
    sf::FloatRect bounds = sprite.getLocalBounds();
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