#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class Button {
public:
    Button(sf::Vector2f size, sf::Vector2f position,
          sf::Color color, const sf::Font& font, 
          const std::string& text);
    
    void draw(sf::RenderWindow& window) const;
    bool isClicked(const sf::Vector2f& mousePos);
    void update(const sf::Vector2f& mousePos);
    bool isHovered(const sf::Vector2f& mousePos) const;
    void setHoverColor(sf::Color color);
    void setOutlineEnabled(bool enabled);
    void setVisible(bool visible);
    bool isVisible() const;
    sf::FloatRect getGlobalBounds() const;

private:
    void centerTextOnButton();
    
    // Visual elements
    sf::RectangleShape shape;
    sf::Text text;  // Changed from buttonText to text
    
    // Audio elements
    sf::SoundBuffer clickBuffer;
    std::unique_ptr<sf::Sound> clickSound;
    
    // State tracking
    bool clicked = false;
    bool wasMousePressedOverButton = false;
    
    // Color properties
    sf::Color normalColor;
    sf::Color hoverColor;
    bool hasOutline = false;
    bool visible = true;
};