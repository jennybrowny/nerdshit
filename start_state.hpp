#pragma once
#include "states/game_state.hpp"
#include "../button.hpp"
#include <vector>
#include <ctime>

struct SaveSlot {
    int slotNumber;
    bool exists;
    int act;
    int scene;
    std::time_t saveTime;
    std::string displayText;
    std::string timeText;
};

class StartState : public GameState {
public:
    explicit StartState(sf::Font& font);
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
    
private:
    void loadSaveSlots();
    bool hasAnySaves() const;
    
    std::unique_ptr<Button> startButton; // Button to start the game
    std::unique_ptr<Button> creditsButton; // Button for credits
    std::unique_ptr<Button> deleteButton; // Button to clear all saves
    
    std::vector<SaveSlot> saveSlots;
    int selectedSaveSlot;
};