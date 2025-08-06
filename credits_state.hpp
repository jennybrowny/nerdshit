// credits because this nerd shit is taking way too damn long and just to fuck with mangoman 
#pragma once
#include "states/game_state.hpp"
#include "../button.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class CreditsState : public GameState {
public:
    explicit CreditsState(sf::Font& font);
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
    
private:
    void createRainbowBackground();
    void updateRainbow();

    sf::Text creditsText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::VertexArray rainbow;
    float gradientPosition;
    std::vector<sf::Color> rainbowColors;
    std::unique_ptr<Button> backButton;
    
};