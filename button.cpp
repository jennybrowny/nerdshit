#include "button.hpp"

Button::Button(sf::Vector2f size, sf::Vector2f position, sf::Color color,
               sf::Font& font, const std::string& text, sf::SoundBuffer& buffer)
    : normalColor(color),
      hoverColor(color.r + 50, color.g + 50, color.b + 50),
      pressedColor(color.r - 50, color.g - 50, color.b - 50),
      clickSound(new sf::Sound(buffer)) 
{
    buttonShape.setSize(size);
    buttonShape.setPosition(position);
    buttonShape.setFillColor(normalColor);
    
    buttonText = new sf::Text(font);
    buttonText->setString(text);
    buttonText->setCharacterSize(24);
    buttonText->setFillColor(sf::Color::White);
    
    sf::FloatRect textBounds = buttonText->getLocalBounds();
    buttonText->setOrigin({
        textBounds.size.x/2.0f,
        textBounds.size.y/2.0f
    });
    buttonText->setPosition({
        position.x + size.x/2.0f,
        position.y + size.y/2.0f
    });
}

Button::~Button() {
    delete buttonText;
    delete clickSound;
}

void Button::update(const sf::Vector2f& mousePos) {
    isHovered = buttonShape.getGlobalBounds().contains(mousePos);
    buttonShape.setFillColor(isHovered ? hoverColor : normalColor);
}

bool Button::isClicked(sf::Vector2f mousePos) {
    if (buttonShape.getGlobalBounds().contains(mousePos)) {
        clickSound->play();
        if (onClick) onClick();
        return true;
    }
    return false;
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(buttonShape);
    if (buttonText) window.draw(*buttonText);
}