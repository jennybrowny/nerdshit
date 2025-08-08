#pragma once
#include "game_state.hpp"
#include "start_state.hpp"
#include "../button.hpp"
#include "audio_manager.hpp"
#include <algorithm>
#include <memory>

class TutorialState : public GameState {
public:
    explicit TutorialState(Game& game);
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
    
private:
    void updateButtonVisibility();
    void handleButtonClicks(Game& game, const sf::Vector2f& mousePos);
    void advancePage();
    void goBackPage();

    std::vector<sf::Texture> tutorialTextures;
    static const inline std::vector<std::string> tutorialPaths = {
        "assets/ACT0/ACT01_0.PNG",
        "assets/ACT0/ACT01_1.PNG",
        "assets/ACT0/ACT01_2.PNG", 
        "assets/ACT0/ACT01_3.PNG"
    };
    
    size_t currentIndex = 0;
    sf::Vector2f lastMousePos;
    
    std::unique_ptr<Button> nextButton;
    std::unique_ptr<Button> prevButton;
    std::unique_ptr<Button> escButton;
    std::unique_ptr<Button> proceedButton;
};