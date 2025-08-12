#pragma once
#include "game_state.hpp"  
#include "start_state.hpp" 
#include "button.hpp"
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>


class CreditsState : public GameState {
public:
    explicit CreditsState(sf::Font& font);
    
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;

private:
    // UI elements
    sf::Text creditsText;
    std::unique_ptr<Button> backButton;
    std::unique_ptr<Button> exitButton;
    
    // Dynamic backgrounds
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    // Animation systems
    sf::VertexArray rainbow;
    std::vector<sf::Color> rainbowColors;
    float gradientPosition;
    
    // NEW: Multiple background variants
    int backgroundVariant;
    float animationTimer;
    
    // Variant-specific elements
    std::vector<sf::CircleShape> floatingOrbs;
    std::vector<sf::Vector2f> orbSpeeds;
    sf::VertexArray leftSpiral;
    sf::VertexArray rightSpiral;
    std::vector<sf::Vertex> cornerParticles;
    sf::VertexArray topWave;
    sf::VertexArray bottomWave;
    
    // Helper methods
    void createDynamicBackground();
    void createFloatingOrbsBackground();
    void createSpiralBackground();
    void createParticleFieldBackground();
    void createWaveBackground();
    void createRainbowBackground();
    
    void updateFloatingOrbs();
    void updateSpirals();
    void updateParticleField();
    void updateWaves();
    void updateRainbow();
    
    void renderFloatingOrbs(Game& game);
    void renderSpirals(Game& game);
    void renderParticleField(Game& game);
    void renderWaves(Game& game);
    
    sf::Vector2f calculateSafeButtonArea();
    sf::Vector2f calculateSafeTextPosition();
};