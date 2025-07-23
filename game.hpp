#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>  // Add this for music support
#include <iostream>

enum GameState {
    START_SCREEN,
    TUTORIAL_SCREEN,
    QUIZ_SCREEN
};

class Button;

class Game {
public:
    Game();
    ~Game();
    void run();
    
private:
    sf::RenderWindow window;
    sf::Texture titleTexture;
    sf::Sprite* titleSprite;
    Button* startButton;
    sf::Color bgColor;
    sf::Font font;
    GameState currentScreen;
    
    // Music system (corrected syntax)
    sf::Music introMusic;  // Fixed: sf::Music with ::
    bool isMusicPlaying;
};