// button.cpp
#include "button.hpp"
#include <iostream>

Button::Button(sf::Vector2f size, sf::Vector2f position, 
              sf::Color color, const sf::Font& font, 
              const std::string& label)
    : shape(),
      text(font, label, 24), // Initialize with label, font and size
      clickSound(),
      normalColor(color),
      hoverColor(color.r + 30, color.g + 30, color.b + 30)
{
    // Setup button shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(normalColor);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    // Setup text properties
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    
    // Center the text on the button
    centerTextOnButton();

    // Load click sound
    if (clickBuffer.loadFromFile("assets/sounds/button_click.mp3")) {
        clickSound = std::make_unique<sf::Sound>(clickBuffer);
        clickSound->setVolume(70);
    } else {
        std::cerr << "ERROR: Button sound not loaded!\n";
    }
}


void Button::centerTextOnButton() {
    // Get the text bounds (SFML 3.0 style)
    auto [position, size] = text.getLocalBounds();
    
    // Calculate origin (center point)
    text.setOrigin({
        position.x + size.x / 2.0f,
        position.y + size.y / 2.0f
    });
    
    // Calculate button center
    sf::Vector2f buttonCenter = {
        shape.getPosition().x + shape.getSize().x / 2.0f,
        shape.getPosition().y + shape.getSize().y / 2.0f
    };
    
    // Set text position
    text.setPosition(buttonCenter);
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


void Button::update(const sf::Vector2f& mousePos) {  // makes it pretty when you hover over it
    if (isHovered(mousePos)) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(normalColor);
    }
}

bool Button::isHovered(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}