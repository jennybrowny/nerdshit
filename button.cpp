#include "button.hpp"
#include <iostream>

Button::Button(sf::Vector2f size, sf::Vector2f position, 
              sf::Color color, const sf::Font& font, 
              const std::string& label)
    : shape(),
      text(font, label, 24), 
      normalColor(color),
      hoverColor(color.r + 30, color.g + 30, color.b + 30)
{
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(normalColor);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    centerTextOnButton();

    if (clickBuffer.loadFromFile("assets/sounds/button_click.mp3")) {
        clickSound = std::make_unique<sf::Sound>(clickBuffer);
        clickSound->setVolume(50);
    } else {
        std::cerr << "ERROR: Button sound not loaded!\n";
    }
}

void Button::centerTextOnButton() {
    auto [position, size] = text.getLocalBounds();
    
    text.setOrigin({
        position.x + size.x / 2.0f,
        position.y + size.y / 2.0f
    });
    
    sf::Vector2f buttonCenter = {
        shape.getPosition().x + shape.getSize().x / 2.0f,
        shape.getPosition().y + shape.getSize().y / 2.0f
    };
    
    text.setPosition(buttonCenter);
}

void Button::draw(sf::RenderWindow& window) const {
    if (!visible) return;
    
    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(const sf::Vector2f& mousePos) {
    if (!visible) return false;
    
    bool isMouseOver = shape.getGlobalBounds().contains(mousePos);
    bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    
    bool isClick = isMouseOver && isMousePressed && !wasMousePressedOverButton;
    wasMousePressedOverButton = isMouseOver && isMousePressed;
    
    if (isClick && clickSound) {
        clickSound->play();
    }
    
    return isClick;
}

void Button::update(const sf::Vector2f& mousePos) {
    if (!visible) return;
    
    if (isHovered(mousePos)) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(normalColor);
    }
}

bool Button::isHovered(const sf::Vector2f& mousePos) const {
    if (!visible) return false;
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setHoverColor(sf::Color color) {
    hoverColor = color;
}

void Button::setOutlineEnabled(bool enabled) {
    hasOutline = enabled;
    shape.setOutlineThickness(enabled ? 2.f : 0.f);
    shape.setOutlineColor(sf::Color::White);
}

void Button::setVisible(bool visible) {
    this->visible = visible;
}

bool Button::isVisible() const {
    return visible;
}

sf::FloatRect Button::getGlobalBounds() const {
    return shape.getGlobalBounds();
}