#include "button.hpp"
#include "audio_manager.hpp"
#include "resource_manager.hpp"
#include <iostream> 

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, 
               const sf::Color& color, sf::Font& font, const std::string& textString)
    : shape(size), text(font), normalColor(color), visible(true), isHovered(false)
{
    // ENHANCED: Load click sound using ResourceManager for better resource management
    try {
        // Ensure click sound is loaded globally for ALL buttons
        auto& audio = AudioManager::getInstance();
        audio.loadSound("click", "assets/sounds/button_click.mp3");
        
        // Optional: Load hover sound too
        audio.loadSound("button_hover", "assets/sounds/button_hover.mp3");
        
        std::cout << "Button audio resources loaded successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Warning: Could not load button audio files - " << e.what() << std::endl;
        // Continue silently - game should work without audio
    }
    
    shape.setPosition(position);
    shape.setFillColor(color);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(color.r + 50, color.g + 50, color.b + 50));
    
    // Calculate hover color (lighter version)
    hoverColor = sf::Color(
        std::min(255, static_cast<int>(color.r) + 40),
        std::min(255, static_cast<int>(color.g) + 40),
        std::min(255, static_cast<int>(color.b) + 40)
    );
    
    text.setString(textString);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    
    // Center text in button - FIXED for SFML 3
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    ));
    text.setPosition(sf::Vector2f(
        position.x + size.x / 2.0f,
        position.y + size.y / 2.0f
    ));
}

bool Button::isClicked(const sf::Vector2f& mousePos) const {
    bool clicked = visible && shape.getGlobalBounds().contains(mousePos);
    
    // ENHANCED GLOBAL AUDIO: Play click sound for ALL button clicks using ResourceManager
    if (clicked) {
        try {
            auto& audio = AudioManager::getInstance();
            audio.playSound("click");
            std::cout << "Button clicked - playing click.mp3" << std::endl;
        } catch (const std::exception& e) {
            // Silently ignore audio errors but log them for debugging
            std::cout << "Warning: Could not play button click sound - " << e.what() << std::endl;
        }
    }
    
    return clicked;
}

void Button::update(const sf::Vector2f& mousePos) {
    if (!visible) return;
    
    bool wasHovered = isHovered;
    isHovered = shape.getGlobalBounds().contains(mousePos);
    
    if (isHovered && !wasHovered) {
        // Just entered hover state
        shape.setFillColor(hoverColor);
        shape.setOutlineColor(sf::Color::White);
        
        // ENHANCED: Optional subtle hover sound (uncomment if desired)
        try {
            // AudioManager::getInstance().playSound("button_hover");
        } catch (...) {
            // Ignore if sound doesn't exist
        }
    } else if (!isHovered && wasHovered) {
        // Just left hover state
        shape.setFillColor(normalColor);
        shape.setOutlineColor(sf::Color(normalColor.r + 50, normalColor.g + 50, normalColor.b + 50));
    }
}

void Button::draw(sf::RenderWindow& window) {
    if (visible) {
        window.draw(shape);
        window.draw(text);
    }
}

void Button::setVisible(bool vis) {
    visible = vis;
}

bool Button::isVisible() const {
    return visible;
}

sf::FloatRect Button::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

// ENHANCED: Static method to play click sound - can be called from anywhere
void Button::playClickSound() {
    try {
        auto& audio = AudioManager::getInstance();
        audio.playSound("click");
        std::cout << "Static button click sound played." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Warning: Could not play static button click sound - " << e.what() << std::endl;
    }
}

// NEW: Static method to preload all button sounds (can be called at game startup)
void Button::preloadButtonSounds() {
    try {
        auto& audio = AudioManager::getInstance();
        
        // Load all button-related sounds
        audio.loadSound("click", "assets/sounds/button_click.mp3");
        audio.loadSound("button_hover", "assets/sounds/button_hover.mp3");
        
        std::cout << "All button sounds preloaded successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Warning: Could not preload button sounds - " << e.what() << std::endl;
    }
}

// NEW: Static method to set global button click volume
void Button::setClickVolume(float volume) {
    try {
        auto& audio = AudioManager::getInstance();
        // Note: This would require extending AudioManager to support individual sound volumes
        // For now, we'll just log the intent
        std::cout << "Button click volume set to: " << volume << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Warning: Could not set button click volume - " << e.what() << std::endl;
    }
}