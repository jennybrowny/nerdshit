#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "button.hpp"

enum ScreenState {
    START_SCREEN,
    TUTORIAL_SCREEN,
    QUIZ_SCREEN
};

class Game {
private:
    // title 
    sf::RenderWindow window;
    sf::Texture titleTexture;
    sf::Sprite* titleSprite;
    // tutorial 
    std::vector<sf::Texture> tutorialTextures;
    sf::Sprite currentTutorialSprite;
    unsigned int currentTutorialIndex;
    // buttons
    Button* startButton;
    sf::Color bgColor;
    // states
    ScreenState currentScreen;
    // music 
    sf::Music introMusic;
    bool isMusicPlaying;
    sf::Font font;

public:
    Game();
    ~Game();
    void run();
};