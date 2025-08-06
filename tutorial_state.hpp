#pragma once
#include "game_state.hpp"
#include "start_state.hpp"
#include "../button.hpp"

class TutorialState : public GameState {
public:
    explicit TutorialState(Game& game);
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
    
private:
    std::unique_ptr<Button> nextButton;
    std::unique_ptr<Button> prevButton;
    std::unique_ptr<Button> escButton;  // Add this line
    std::vector<sf::Texture>& tutorialTextures;
    unsigned int currentIndex = 0;
};