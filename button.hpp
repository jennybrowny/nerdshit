#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>

class Button {
public:
    Button(sf::Vector2f size, sf::Vector2f position, sf::Color color,
           sf::Font& font, const std::string& text, sf::SoundBuffer& buffer);
    ~Button();
    
    void draw(sf::RenderWindow& window);
    void update(const sf::Vector2f& mousePos);
    bool isClicked(sf::Vector2f mousePos);
    
    void setOnClick(std::function<void()> callback) { onClick = callback; }
    void setHoverColor(sf::Color color) { hoverColor = color; }
    void setPressedColor(sf::Color color) { pressedColor = color; }

private:
    sf::RectangleShape buttonShape;
    sf::Text* buttonText;
    sf::Sound* clickSound;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    std::function<void()> onClick;
    bool isHovered = false;
};