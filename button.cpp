#include "button.hpp"

// Initialize static sound variables
sf::SoundBuffer SoundManager::clickBuffer;
bool SoundManager::soundLoaded = false;

// ===== ButtonVisual Methods ===== //

ButtonVisual::ButtonVisual(sf::Vector2f size, sf::Vector2f position,
                         const sf::Color& color, const sf::Font& font,
                         const std::string& label)
    : normalColor(color), hoverColor(color)
{
    // Setup shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(normalColor);
    
    // Setup text (SFML 3.0 style)
    text = std::make_unique<sf::Text>(); //change line to have font
    text->setFont(font);
    text->setString(label);
    text->setCharacterSize(24);
    text->setFillColor(sf::Color::White);
    
    // Center text (SFML 3.0 compatible)
    sf::FloatRect bounds = text->getLocalBounds();
    text->setOrigin({
        bounds.getPosition().x + bounds.getSize().x / 2.0f,
        bounds.getPosition().y + bounds.getSize().y / 2.0f
    });
    text->setPosition(position + size / 2.0f);
}

void ButtonVisual::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(*text);
}

bool ButtonVisual::contains(sf::Vector2f point) const {
    return shape.getGlobalBounds().contains(point);
}

// ===== SoundManager Methods ===== //

SoundManager::SoundManager() : clickSound(clickBuffer) {
    if (!soundLoaded) {
        if (!clickBuffer.loadFromFile("assets/sounds/button_click.mp3")) {
            throw std::runtime_error("Failed to load button sound");
        }
        soundLoaded = true;
    }
}

void SoundManager::playClick(float volume) {
    clickSound.setVolume(volume);
    clickSound.play();
}

// ===== Button Methods ===== //

Button::Button(sf::Vector2f size, sf::Vector2f position,
              const sf::Color& color, const sf::Font& font,
              const std::string& text)
    : ButtonVisual(size, position, color, font, text) {}

void Button::onClick() {
    AudioManager::getInstance().playSound("click");
}

void Button::onHover() {
    setHoverState(true);
}

void Button::draw(sf::RenderWindow& window) const {
    ButtonVisual::draw(window);
}

bool Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto& mouseEvent = event.get<sf::Event::MouseButtonPressed>();
        sf::Vector2f mousePos = window.mapPixelToCoords(
            {mouseEvent.x, mouseEvent.y});
        
        if (contains(mousePos)) {
            onClick();
            return true;
        }
    }
    else if (event.is<sf::Event::MouseMoved>()) {
        auto& mouseEvent = event.get<sf::Event::MouseMoved>();
        sf::Vector2f mousePos = window.mapPixelToCoords(
            {mouseEvent.x, mouseEvent.y});
        
        if (contains(mousePos)) {
            onHover();
        } else {
            setHoverState(false);
        }
    }
    return false;
}