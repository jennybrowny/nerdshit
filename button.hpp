// button.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class Button {
public:
    Button(sf::Vector2f size, sf::Vector2f position, // Constructor: size, position, color, font, label
          sf::Color color, const sf::Font& font, 
          const std::string& label);
    
    void draw(sf::RenderWindow& window) const; // Draw the button on the window
    bool isClicked(const sf::Vector2f& mousePos); // Check if button was clicked
    void update(const sf::Vector2f& mousePos); // Update button appearance based on mouse position
    bool isHovered(const sf::Vector2f& mousePos) const;  // Check if mouse is over the button   
    void setHoverColor(sf::Color color) { hoverColor = color; }


private:
    void centerTextOnButton(); // Center text on the button
    
    sf::RectangleShape shape; // Button shape
    sf::Text text; // Button label text
    sf::SoundBuffer clickBuffer; // Sound buffer for click sound
    std::unique_ptr<sf::Sound> clickSound; // Click sound effect
    bool clicked = false; // Click state flag
    sf::Color normalColor; /// Normal button color
    sf::Color hoverColor; // Color when hovered over
};