#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;
    
    sf::Color normalColor;
    sf::Color hoverColor;
    bool isHovered = false;

public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position,
          const sf::Color& color, const sf::Font& font,
          const std::string& label);

    void draw(sf::RenderWindow& window) const;
    bool isClicked(const sf::Vector2f& mousePos) const;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    std::function<void()> onClick;
};