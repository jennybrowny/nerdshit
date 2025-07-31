#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "button.hpp"
#include "audio_manager.hpp"  
#include <memory>
#include "states/game_state.hpp"
#include <memory>
/*----------------------------------------------
 *  MAIN GAME CLASS
 *  Manages all game systems and rendering
 *--------------------------------------------*/

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
    sf::Sprite currentTutorialSprite;          // Currently displayed slide
    unsigned int currentTutorialIndex;         // Current slide position
    
    //=========== UI ELEMENTS ===========//
    std::unique_ptr<Button> startButton; // Begins the tutorial
    std::unique_ptr<Button> nextButton;  // Advances slides
    std::unique_ptr<Button> prevButton;  // Goes back slides

    //=========== AUDIO SYSTEM ===========//
    sf::Music introMusic;       // Background music
    bool isMusicPlaying;        // Music state flag
    
    //=========== TEXT SYSTEM ===========//
    sf::Font font;              // Main UI font
    sf::Text pageText;          // "X/Y" page counter

    //---------- PRIVATE HELPERS ----------//
    // Scales sprites to fit window dimensions
    void scaleSpriteToWindow(sf::Sprite& sprite);
    
    // Updates the "X/Y" page indicator text
    void updatePageIndicator();
    std::unique_ptr<GameState> currentState;


public:
    //=========== LIFECYCLE METHODS ===========//
    Game();   // Constructor - Initializes all systems
    ~Game();  // Destructor  - Cleans up resources
    void run(); // Main game loop

    //=========== RESOURCE MANAGEMENT ===========//
    void loadResources();   // Loads textures, fonts, audio
    void createButtons();   // Sets up UI buttons

    //=========== GAME LOOP SYSTEMS ===========//
    void handleEvents();  // Processes SFML events (input etc)
    void update();        // Game state updates (logic)
    void render();        // Draws everything to window

    //=========== TUTORIAL NAVIGATION ===========//
void goToPreviousPage();
void changeState(std::unique_ptr<GameState> newState);






// adding game state management
    sf::RenderWindow& getWindow() { return window; }
    sf::Font& getFont() { return font; }
    std::unique_ptr<sf::Sprite>& getTitleSprite() { return titleSprite; }
    std::vector<sf::Texture>& getTextures() { return tutorialTextures; }
    void changeState(std::unique_ptr<GameState> newState) {
        currentState = std::move(newState);
    }
};

