// HOW TO RUN THIS PROGRAM ON YOUR NATIVE PC:
// 1. Install SFML library on your computer
//    - Windows: Download SFML from sfml-dev.org
//    - Mac: Use Homebrew: "brew install sfml"  
//    - Linux: Use package manager: "sudo apt-get install libsfml-dev"
//
// 2. Compile with this command in terminal:
//    g++ -o game main.cpp Game.cpp Player.cpp QuizManager.cpp Button.cpp -lsfml-graphics -lsfml-window -lsfml-system
//
// 3. Run the game:
//    ./game (on Mac/Linux)
//    game.exe (on Windows)
//
// NOTE: You'll also need a font file called "arial.ttf" in the same folder
// Download any .ttf font file and rename it to arial.ttf
#include <iostream>
#include "Game.h"

// MAIN FUNCTION - This is where the program starts
// Think of this as the "power button" for your game
int main() {
    std::cout << "=== Starting nerdshit: Engineering Survival Game ===" << std::endl;
    std::cout << "Game Controls:" << std::endl;
    std::cout << "- Click buttons with your mouse" << std::endl;
    std::cout << "- Press SPACE to advance through story scenes" << std::endl;
    std::cout << "- Press ESC to return to main menu anytime" << std::endl;
    std::cout << "- Close the window or press ALT+F4 to quit" << std::endl;
    std::cout << std::endl;
    std::cout << "Loading game..." << std::endl;

    try {
        // Create the main game object and start it up
        // This is like turning on a game console
        Game game;
        game.run();  // Start the main game loop

    } catch (const std::exception& e) {
        // If something goes wrong, tell the user
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "Make sure you have SFML installed and arial.ttf font file!" << std::endl;
        return -1;
    }

    std::cout << "Thanks for playing nerdshit! Hope you survived engineering!" << std::endl;
    return 0;
}