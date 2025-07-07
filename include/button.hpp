#pragma once
#include "/Users/jennybrown/Desktop/SFML/include/SFML/Graphics.hpp"
#include "/Users/jennybrown/Desktop/SFML/include/SFML/Graphics/BlendMode.hpp"
#include <string>

// Button class encapsulates a clickable rectangular button with text for SFML windows.
class Button {
public:
    // Constructor: initializes button size, position, fill color, outline color, and outline thickness.
    Button(const sf::Vector2f& size, const sf::Vector2f& pos, 
           const sf::Color& fill, const sf::Color& outline, float outlineThickness);
    
    // Sets the button's text, font, character size, color, and style.
    void setText(const std::string& text, sf::Font& font, unsigned int charSize, 
                 const sf::Color& color, sf::Text::Style style);
    
    // Draws the button (rectangle and text) to the given window.
    void draw(sf::RenderWindow& window) const;

    // Returns true if the given mouse position is inside the button's bounds.
    bool isClicked(const sf::Vector2i& mousePos) const;

private:
    sf::RectangleShape shape_; // The rectangle shape representing the button.
    sf::Text text_;            // The text displayed on the button.
};