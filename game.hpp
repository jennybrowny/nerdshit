#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;  // Will be initialized with buffer
    sf::Color bgColor;
    sf::Font font;
    GameState currentScreen;
    sf::Music introMusic;
    bool isMusicPlaying;
 // Tutorial System
    std::vector<sf::Texture> tutorialTextures;  // Stores ACT0 images
    sf::Sprite currentTutorialSprite;           // Current fullscreen image
    sf::Sound tutorialSound;                    // Single shared sound
    int currentTutorialIndex = 0;               // Tracks image progress           // Which picture we're showing
};