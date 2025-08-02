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
    
    //=========== UI ELEMENTS ===========//
    std::unique_ptr<Button> startButton; // Begins the tutorial
    std::unique_ptr<Button> nextButton;  // Advances slides
    std::unique_ptr<Button> prevButton;  // Goes back slides

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
    void loadResources();
    void createButtons();

    //=========== GAME LOOP SYSTEMS ===========//
    void handleEvents();
    void update();
    void render();

    //=========== TUTORIAL NAVIGATION ===========//
    void goToPreviousPage();
    void goToNextPage();
    void changeState(std::unique_ptr<GameState> newState);

    //=========== GETTERS ===========//
    sf::RenderWindow& getWindow() { return window; }
    sf::Font& getFont() { return font; }
    std::unique_ptr<sf::Sprite>& getTitleSprite() { return titleSprite; }
    std::vector<sf::Texture>& getTextures() { return tutorialTextures; }
    std::unique_ptr<sf::Sprite>& getCurrentTutorialSprite() { return currentTutorialSprite; }
    AudioManager& getAudioManager() { return AudioManager::getInstance(); }
};