
#include "Button.h"

// CONSTRUCTOR - Sets up a new button
Button::Button(float x, float y, float width, float height, 
               const std::string& text, sf::Font* gameFont) {
    // Set up the rectangle shape for the button
    buttonShape.setPosition(x, y);
    buttonShape.setSize(sf::Vector2f(width, height));
    
    // Set up the text
    font = gameFont;
    buttonText.setFont(*font);
    buttonText.setString(text);
    buttonText.setCharacterSize(16);
    buttonText.setFillColor(sf::Color::Black);
    
    // Center the text in the button
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        x + (width - textBounds.width) / 2,
        y + (height - textBounds.height) / 2
    );
    
    // Set default colors
    normalColor = sf::Color::White;
    hoverColor = sf::Color::Yellow;
    clickColor = sf::Color::Green;
    buttonShape.setFillColor(normalColor);
    
    // Initialize states
    isPressed = false;
    isHovered = false;
}

// SET COLORS - Change button colors for different states
void Button::setColors(sf::Color normal, sf::Color hover, sf::Color click) {
    normalColor = normal;
    hoverColor = hover;
    clickColor = click;
}

// UPDATE - Check mouse position and handle button states
void Button::update(sf::Vector2i mousePos, bool mouseClicked) {
    sf::FloatRect buttonBounds = buttonShape.getGlobalBounds();
    
    // Check if mouse is over the button
    isHovered = buttonBounds.contains(static_cast<float>(mousePos.x), 
                                      static_cast<float>(mousePos.y));
    
    // Update button state and color
    if (isHovered && mouseClicked) {
        isPressed = true;
        buttonShape.setFillColor(clickColor);
    } else if (isHovered) {
        buttonShape.setFillColor(hoverColor);
    } else {
        buttonShape.setFillColor(normalColor);
    }
}

// DRAW - Render the button to the screen
void Button::draw(sf::RenderWindow& window) {
    window.draw(buttonShape);
    window.draw(buttonText);
}

// WAS CLICKED - Check if button was clicked (and reset the pressed state)
bool Button::wasClicked() {
    if (isPressed) {
        isPressed = false;  // Reset so it only returns true once per click
        return true;
    }
    return false;
}

// IS MOUSE OVER - Check if mouse is currently hovering over button
bool Button::isMouseOver() {
    return isHovered;
}

// SET TEXT - Change the text displayed on the button
void Button::setText(const std::string& text) {
    buttonText.setString(text);
    
    // Recenter the text
    sf::FloatRect buttonBounds = buttonShape.getGlobalBounds();
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        buttonBounds.left + (buttonBounds.width - textBounds.width) / 2,
        buttonBounds.top + (buttonBounds.height - textBounds.height) / 2
    );
}
