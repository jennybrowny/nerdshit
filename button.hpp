#pragma once
#include <SFML/Graphics.hpp>

class Button {
public:
    // add font parameter to constructor
    Button(sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Font& font, std::string text);
    ~Button();
    
    void draw(sf::RenderWindow& window);
    bool isClicked(sf::Vector2f mousePos);
    
private:
    sf::RectangleShape buttonShape;
    sf::Text* buttonText;        // changed to pointer for sfml 3.0
    sf::Color buttonColor;
};