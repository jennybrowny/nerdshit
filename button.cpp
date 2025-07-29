#include "button.hpp"
#include <iostream>

// PROPERLY INITIALIZE ALL MEMBERS THAT NEED CONSTRUCTION
Button::Button(sf::Vector2f size, sf::Vector2f position, 
              sf::Color color, const sf::Font& font, 
              const std::string& label)
    : text(font, label, 18),       // Initialize text first
      clickSound()                 // Then sound (buffer loaded later)
{
    // Setup shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(color);

    // Position text
    text.setPosition(sf::Vector2f(position.x + 10, position.y + 5));

    // Load sound

// Sound fix:
if (clickBuffer.loadFromFile("assets/sounds/button_click.mp3")) {
        clickSound = std::make_unique<sf::Sound>(clickBuffer);  // Create WITH buffer
        clickSound->setVolume(70);
    } else {
        std::cerr << "ERROR: Button sound not loaded!\n";
    }
}

bool Button::isClicked(const sf::Vector2f& mousePos) {  // Must match header
    clicked = shape.getGlobalBounds().contains(mousePos);
    if (clicked && clickSound) {  // Check both click state and sound
        clickSound->play();
    }
    return clicked;
}