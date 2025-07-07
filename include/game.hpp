#pragma once
#include <SFML/Graphics.hpp>
#include "button.hpp"

// Game class manages the main application window, resources, and game loop.
class Game {
public:
    // Constructor: sets up the window and initializes resources.
    Game();
    // Starts the main game loop.
    void run();

private:
    // Handles window and input events.
    void processEvents();
    // Updates game state (stub for future logic).
    void update();
    // Renders all game objects to the window.
    void render();
    // Loads textures, fonts, and other resources.
    void loadResources();
    
    sf::RenderWindow window_;      // Main application window.
    sf::Texture backgroundTexture_;// Texture for the background image.
    sf::Sprite backgroundSprite_;  // Sprite to display the background.
    sf::Font font_;                // Font used for button text.
    Button startButton_;           // Start button displayed on the screen.
    bool backgroundLoaded_;        // Flag indicating if background was loaded successfully.
};