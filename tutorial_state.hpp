#pragma once
#include "game_state.hpp"
#include <vector>

class TutorialState : public GameState {
public:
    explicit TutorialState(Game& game);
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;

private:
    std::vector<sf::Texture>& tutorialTextures;
    unsigned currentIndex = 0;
    std::unique_ptr<Button> nextButton;
    std::unique_ptr<Button> prevButton;
};