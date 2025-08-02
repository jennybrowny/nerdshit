#pragma once
#include "states/game_state.hpp"
#include "../button.hpp"

class StartState : public GameState {
public:
    explicit StartState(sf::Font& font);
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
private:
    std::unique_ptr<Button> startButton;
};