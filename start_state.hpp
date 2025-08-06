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
    std::unique_ptr<Button> startButton; // Button to start the game
    std::unique_ptr<Button> saveButton;  // Button to save the game state (placeholder)
    // button for credits cause why not
    std::unique_ptr<Button> creditsButton; // Add this line
    // Add any other UI elements needed for the start state
};