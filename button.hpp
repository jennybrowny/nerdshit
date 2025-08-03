// button.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class Button {
public:
    Button(sf::Vector2f size, sf::Vector2f position, 
          sf::Color color, const sf::Font& font, 
          const std::string& label);
    
    void draw(sf::RenderWindow& window) const;
    bool isClicked(const sf::Vector2f& mousePos);
    void update(const sf::Vector2f& mousePos);

private:
    void centerTextOnButton();
    
    sf::RectangleShape shape;
    sf::Text text;
    sf::SoundBuffer clickBuffer;
    std::unique_ptr<sf::Sound> clickSound;
    bool clicked = false;
    sf::Color normalColor;
    sf::Color hoverColor;
};