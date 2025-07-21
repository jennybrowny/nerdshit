
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

// BUTTON CLASS - Makes clickable buttons for the game
// Think of this like creating a button widget that you can click
class Button {
private:
    sf::RectangleShape buttonShape;  // The rectangle that shows the button
    sf::Text buttonText;             // The text that goes on the button
    sf::Font* font;                  // Pointer to the font (shared with game)
    sf::Color normalColor;           // Button color when not clicked
    sf::Color hoverColor;           // Button color when mouse is over it
    sf::Color clickColor;           // Button color when being clicked
    bool isPressed;                 // Is the button currently being clicked?
    bool isHovered;                 // Is the mouse over the button?
    
public:
    // Constructor - creates a button with position, size, text, and colors
    Button(float x, float y, float width, float height, 
           const std::string& text, sf::Font* gameFont);
    
    // Set the colors for different button states
    void setColors(sf::Color normal, sf::Color hover, sf::Color click);
    
    // Update button state based on mouse position and clicks
    void update(sf::Vector2i mousePos, bool mouseClicked);
    
    // Draw the button to the screen
    void draw(sf::RenderWindow& window);
    
    // Check if button was clicked this frame
    bool wasClicked();
    
    // Check if mouse is currently over button
    bool isMouseOver();
    
    // Change the text on the button
    void setText(const std::string& text);
};

#endif
