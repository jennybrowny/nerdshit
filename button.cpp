#include "button.hpp"

// biggest issues atm 7/22: font & auto placement
// to do if thereʻs time, load up with font 
// have the position work with SFML 3.0 
Button::Button(sf::Vector2f size, sf::Vector2f position, sf::Color color, 
               sf::Font& font, const std::string& text) 
    : buttonColor(color), buttonText(new sf::Text("", font, 24))  // Initialize with required font
{
    // Set up button shape
    buttonShape.setSize(size);
    buttonShape.setPosition(position);
    buttonShape.setFillColor(color);

    // Configure text properties
    buttonText->setString(text);
    buttonText->setFillColor(sf::Color::White);

    // Calculate text position (SFML 3.0 compatible)
    sf::Vector2f textPosition;
    textPosition.x = position.x + (size.x - buttonText->getLocalBounds().width) / 2.0f;
    textPosition.y = position.y + (size.y - 24) / 2.0f;  // 24 matches character size
    
    buttonText->setPosition(textPosition);
}

Button::~Button() {
    delete buttonText;
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(buttonShape);
    window.draw(*buttonText);
}

bool Button::isClicked(sf::Vector2f mousePos) {
    if (buttonShape.getGlobalBounds().contains(mousePos)) {
        playClickSound();
        return true;
    }
    return false;
}