#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <string>
#include <ctime>

// Forward declarations
class GameState;

class Game {
public:
    Game();
    ~Game();
    
    void run();
    void changeState(std::unique_ptr<GameState> newState);
    
    // Essential getters
    sf::RenderWindow& getWindow() { return window; }
    sf::Font& getFont() { return font; }
    sf::Sprite* getTitleSprite() { return titleSprite.get(); }
    
    // Sprite scaling utilities
    void scaleSpriteToWindow(sf::Sprite& sprite);
    void autoResizeSprite(sf::Sprite& sprite, const sf::Vector2u& targetSize);
    void scaleSpriteToContentArea(sf::Sprite& sprite, float uiReservedHeight = 80.0f);
    
    // Save system
    void saveGame(int act, int scene);
    bool hasSavedGame() const;
    std::pair<int, int> loadSavedGame();
    std::string getSaveTime() const;

private:
    // Core systems
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;
    std::unique_ptr<GameState> currentState;
    
    // Graphics - FIXED: Use smart pointer consistently
    std::unique_ptr<sf::Sprite> titleSprite;
    sf::Texture titleTexture;
    sf::Color bgColor;
    
    // Audio
    bool isMusicPlaying;
    
    // Save data
    std::time_t lastSaveTime;
    
    // Tutorial system
    std::vector<sf::Texture> tutorialTextures;
    std::unique_ptr<sf::Sprite> currentTutorialSprite;
    int currentTutorialIndex;
    sf::Text pageText;
    
    static const std::vector<std::string> fontPaths;
    static const inline std::vector<std::string> tutorialPaths = {
        "assets/ACT0/ACT01_0.PNG",
        "assets/ACT0/ACT01_1.PNG",
        "assets/ACT0/ACT01_2.PNG",
        "assets/ACT0/ACT01_3.PNG"
    };
    
    // Initialization methods
    void loadResources();
    void loadAudioResources();
    void loadVisualResources();
    void loadTitleScreen();
    void loadFont();
    bool tryLoadFont(const std::string& path);
    
    // Event handling
    void handleEvents();
    void update();
    void render();
    
    // Error handling
    void handleAudioLoadError(const std::runtime_error& e);
    void handleVisualLoadError(const std::exception& e);
    void logFontSuccess(const std::string& path);
    
    // Tutorial navigation
    void updatePageIndicator();
    void goToNextPage();
    void goToPreviousPage();
    
    // Window utilities
    sf::Vector2u getCurrentResolution() const;
    bool wasWindowResized();
};