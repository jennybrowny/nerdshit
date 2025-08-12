#pragma once
#include "game_state.hpp"
#include "button.hpp"
#include <memory>

class StartState : public GameState {
public:
    explicit StartState(sf::Font& font);
    
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;

private:
    std::unique_ptr<Button> startButton;
    std::unique_ptr<Button> loadGameButton;
    std::unique_ptr<Button> creditsButton;
    std::unique_ptr<Button> exitButton;
    
    int selectedSaveSlot;
};