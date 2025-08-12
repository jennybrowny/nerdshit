#pragma once
#include "game_state.hpp"
#include "../button.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// Forward declaration
class Game;

class TutorialState : public GameState {
public:
    explicit TutorialState(Game& game);
    ~TutorialState() override = default;

    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;

private:
    // Tutorial textures and navigation
    std::vector<sf::Texture> tutorialTextures;
    size_t currentIndex;
    sf::Vector2f lastMousePos;
    
    // UI elements
    std::unique_ptr<Button> nextButton;
    std::unique_ptr<Button> prevButton;
    std::unique_ptr<Button> escButton;
    std::unique_ptr<Button> proceedButton;
    
    // Tutorial paths
    const std::vector<std::string> tutorialPaths = {
        "assets/ACT0/ACT01_0.PNG",
        "assets/ACT0/ACT01_1.PNG", 
        "assets/ACT0/ACT01_2.PNG",
        "assets/ACT0/ACT01_3.PNG"
    };
    
    // Helper methods
    void updateButtonVisibility();
    void advancePage();
    void goBackPage();
    void handleButtonClicks(Game& game, const sf::Vector2f& mousePos);
};