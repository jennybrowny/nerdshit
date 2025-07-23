#include "button.hpp"

// updated constructor with font and text
Button::Button(sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Font& font, std::string text) 
    : buttonColor(color), buttonText(nullptr) {
    
    // set up the rectangle
    buttonShape.setSize(size);
    buttonShape.setPosition(position);
    buttonShape.setFillColor(color);
    
    // create text object with font (sfml 3.0 way)
    buttonText = new sf::Text(font);
    buttonText->setString(text);
    buttonText->setCharacterSize(24);           // nice readable size
    buttonText->setFillColor(sf::Color::White); // white text on green button
    
    // center the text on the button (fixed for sfml 3.0)
    sf::FloatRect textBounds = buttonText->getLocalBounds();
    sf::Vector2f buttonCenter = position + size / 2.0f;
    
    sf::Vector2f textPos(
        buttonCenter.x - textBounds.size.x / 2.0f,  // use .size.x instead of .width
        buttonCenter.y - textBounds.size.y / 2.0f   // use .size.y instead of .height
    );
    
    buttonText->setPosition(textPos);  // fixed: use Vector2f for sfml 3.0
}

Button::~Button() {
    delete buttonText;  // clean up text memory
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(buttonShape);  // draw button rectangle
    if (buttonText) {
        window.draw(*buttonText);   // draw text on top (dereference pointer)
    }
}

bool Button::isClicked(sf::Vector2f mousePos) {
    return buttonShape.getGlobalBounds().contains(mousePos);
}