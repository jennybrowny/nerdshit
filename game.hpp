#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "button.hpp"
#include "audio_manager.hpp"
#include <memory>
#include "states/game_state.hpp"


class Game {
private:
    //=========== CORE SYSTEMS ===========//
    sf::RenderWindow window;    // Main game window
    sf::Color bgColor;          // Background fallback color
    
    //=========== TITLE SCREEN ===========//
    sf::Texture titleTexture;   // Background image texture
    std::unique_ptr<sf::Sprite> titleSprite; // Smart pointer to title graphic
    
    //=========== TUTORIAL SYSTEM ===========//
    std::vector<sf::Texture> tutorialTextures; // All tutorial slide images
    std::unique_ptr<sf::Sprite> currentTutorialSprite; // Currently displayed slide
    unsigned int currentTutorialIndex;         // Current slide position

    //=========== AUDIO SYSTEM ===========//
    bool isMusicPlaying;        // Music state flag
    
    //=========== TEXT SYSTEM ===========//
    sf::Font font;              // Main UI font
    sf::Text pageText;          // "X/Y" page counter
    bool fontLoaded;            // Track if font was successfully loaded

    // State management
    std::unique_ptr<GameState> currentState;

    //---------- PRIVATE HELPERS ----------//
    void scaleSpriteToWindow(sf::Sprite& sprite);
    void updatePageIndicator();

public:
    //=========== LIFECYCLE METHODS ===========//
    Game();
    ~Game();
    void run();

    //=========== RESOURCE MANAGEMENT ===========//
    void loadResources(); // Load all game resources like textures, fonts, and audio
    //=========== GAME LOOP SYSTEMS ===========//
    void handleEvents(); // Handle user input events
    void update(); // Update game logic
    void render(); // Render graphics to the window

    //=========== TUTORIAL NAVIGATION ===========//
    void goToPreviousPage(); // Navigate to the previous tutorial page
    void goToNextPage(); // Navigate to the next tutorial page
    void changeState(std::unique_ptr<GameState> newState); // Change the current game state

    //=========== GETTERS ===========//
    sf::RenderWindow& getWindow() { return window; }
    sf::Font& getFont() { return font; }
    unsigned int getCurrentTutorialIndex() const { return currentTutorialIndex; } // Get current tutorial index for the save file
    std::unique_ptr<sf::Sprite>& getTitleSprite() { return titleSprite; }
    std::vector<sf::Texture>& getTextures() { return tutorialTextures; }
    std::unique_ptr<sf::Sprite>& getCurrentTutorialSprite() { return currentTutorialSprite; }
    AudioManager& getAudioManager() { return AudioManager::getInstance(); }
};