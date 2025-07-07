#include "/Users/jennybrown/nershit/include/button.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <string>

// Constructor: Initializes the button's shape, position, fill color, outline color, and outline thickness.
Button::Button(const sf::Vector2f& size, const sf::Vector2f& pos, 
               const sf::Color& fill, const sf::Color& outline, float outlineThickness) {
    shape_.setSize(size);
    shape_.setPosition(pos);
    shape_.setFillColor(fill);
    shape_.setOutlineThickness(outlineThickness);
    shape_.setOutlineColor(outline);
}

// Sets the button's text, font, size, color, and style. Also centers the text within the button.
void Button::setText(const std::string& text, sf::Font& font, unsigned int charSize, 
                     const sf::Color& color, sf::Text::Style style) {
    text_.setFont(font);
    text_.setString(text);
    text_.setCharacterSize(charSize);
    text_.setFillColor(color);
    text_.setStyle(style);

    // Center text in button
    sf::FloatRect textRect = text_.getLocalBounds();
    sf::Vector2f btnPos = shape_.getPosition();
    sf::Vector2f btnSize = shape_.getSize();
    text_.setOrigin(sf::Vector2f(textRect.width / 2.f, textRect.height / 2.f));
    text_.setPosition(sf::Vector2f(btnPos.x + btnSize.x / 2.f, btnPos.y + btnSize.y / 2.f));
}

// Draws the button (shape and text) to the given window.
void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape_);
    window.draw(text_);
}

// Returns true if the mouse position is within the button's bounds.
bool Button::isClicked(const sf::Vector2i& mousePos) const {
    return shape_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}