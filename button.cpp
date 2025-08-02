#include "button.hpp"
#include <iostream>

Button::Button(sf::Vector2f size, sf::Vector2f position, 
              sf::Color color, const sf::Font& font, 
              const std::string& label)
    : text(font, label, 18),
      clickSound()
{
    // Setup shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(color);

    // Position text
    text.setPosition(sf::Vector2f(position.x + 10, position.y + 5));

    // Load sound
    if (clickBuffer.loadFromFile("assets/sounds/button_click.mp3")) {
        clickSound = std::make_unique<sf::Sound>(clickBuffer);
        clickSound->setVolume(70);
    } else {
        std::cerr << "ERROR: Button sound not loaded!\n";
    }
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(const sf::Vector2f& mousePos) {
    clicked = shape.getGlobalBounds().contains(mousePos);
    if (clicked && clickSound) {
        clickSound->play();
    }
    return clicked;
}