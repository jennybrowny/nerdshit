
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameState.h"
#include "Player.h"
#include "QuizManager.h"
#include "Button.h"

// MAIN GAME CLASS - Controls everything in the game
// This is like the "director" that manages all the other parts
class Game {
private:
    sf::RenderWindow window;         // The game window
    GameState currentState;          // What screen we're currently showing
    Player player;                   // The student character
    QuizManager quizManager;         // Handles all the quiz questions
    sf::Font font;                   // Font for all text
    sf::Clock loadingClock;          // Timer for loading screen
    
    // BUTTONS - Using our new Button class!
    std::vector<std::unique_ptr<Button>> menuButtons;     // Main menu buttons
    std::vector<std::unique_ptr<Button>> actionButtons;   // Study/Caffeine/ChatGPT buttons
    std::vector<std::unique_ptr<Button>> choiceButtons;   // A/B/C/D quiz buttons
    
    sf::Vector2i mousePosition;      // Current mouse position
    bool mouseClicked;               // Was mouse clicked this frame?
    
public:
    // Constructor - sets up the game window and loads resources
    Game();
    
    // MAIN GAME LOOP - Runs continuously while game is active
    void run();
    
private:
    // CORE GAME FUNCTIONS
    void handleEvents();    // Handle user input (clicks, keys)
    void update();         // Update game logic
    void render();         // Draw everything to screen
    
    // INPUT HANDLING
    void handleMouseClick(int x, int y);
    void handleKeyPress(sf::Keyboard::Key key);
    
    // BUTTON MANAGEMENT
    void createMenuButtons();      // Create main menu buttons
    void createActionButtons();    // Create study/caffeine/chatgpt buttons
    void createChoiceButtons();    // Create A/B/C/D quiz buttons
    void updateButtons();          // Update all buttons each frame
    
    // DRAWING FUNCTIONS - One for each game screen
    void drawLoadingScreen();
    void drawMainMenu();
    void drawScene1();
    void drawScene2ECE160();
    void drawScene2ECE323();
    void drawCredits();
    void drawGameOver();
    
    // UTILITY FUNCTIONS
    void drawPlayerStats();        // Show health and stamina
};

#endif
