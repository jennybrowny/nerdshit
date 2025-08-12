#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, 
           const sf::Color& color, sf::Font& font, const std::string& text);
    
    bool isClicked(const sf::Vector2f& mousePos) const;
    void update(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window);
    void setVisible(bool visible);
    bool isVisible() const;
    sf::FloatRect getGlobalBounds() const;
    
    // ENHANCED: Static methods for global audio management
    static void playClickSound();
    static void preloadButtonSounds();       // NEW: Preload all button sounds at startup
    static void setClickVolume(float volume); // NEW: Set global click volume
    
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    bool visible;
    bool isHovered;
};