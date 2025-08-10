#pragma once
#include "game_state.hpp"
#include "button.hpp"
#include <vector>

class ACT1_state : public GameState {
public:
    explicit ACT1_state(sf::Font& font, int startIndex = 0) noexcept;
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;

private:
    void loadSprites();  // Make sure this is declared
    void updateButtons(); // And this too
    
    sf::Font& font;
    std::vector<sf::Texture> sprites;
    sf::Texture dummyTexture;
    sf::Sprite currentSprite;
    int currentIndex;
    
    std::unique_ptr<Button> nextButton;
    std::unique_ptr<Button> prevButton;
    std::unique_ptr<Button> menuButton;
    std::unique_ptr<Button> saveButton;
    bool showSaveButton;
    
    // Save feedback system
    bool justSaved;
    float saveMessageTimer;
};