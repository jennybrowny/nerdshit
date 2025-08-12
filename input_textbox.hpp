#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class InputTextBox {
private:
    sf::Font& font;
    sf::RectangleShape background;
    sf::RectangleShape cursor;
    sf::Text displayText;
    sf::Text placeholderDisplay;
    
    std::string inputText;
    std::string placeholderText;
    
    int maxCharacters;
    bool isFocused;
    bool isVisible;
    
    // Cursor blinking
    float cursorBlinkTimer;
    bool showCursor;
    
    // Colors
    sf::Color normalColor;
    sf::Color focusedColor;
    sf::Color textColor;
    sf::Color placeholderColor;
    
    // NEW: Enhanced input modes
    bool numericOnly;     // Only allow digits 0-9
    bool rightAligned;    // Right-align text within the box
    
    void handleTextEntered(std::uint32_t unicode);
    void updateDisplayText();

public:
    InputTextBox(const sf::Vector2f& size, const sf::Vector2f& position, 
                 sf::Font& font, int maxCharacters = 50);
    
    void handleEvent(const sf::Event& event);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    // Text management
    void setText(const std::string& text);
    std::string getText() const { return inputText; }
    void clear() { inputText.clear(); updateDisplayText(); }
    
    // Position and bounds
    void setPosition(const sf::Vector2f& pos);
    sf::FloatRect getGlobalBounds() const;
    
    // Appearance
    void setPlaceholderText(const std::string& placeholder);
    
    // Focus management
    void setFocused(bool focused) { isFocused = focused; }
    bool isFocusedBox() const;  // NEW: External focus checking
    
    // Visibility
    void setVisible(bool visible) { isVisible = visible; }
    bool getVisible() const { return isVisible; }
    
    // NEW: Input mode configuration
    void setNumericOnly(bool numeric);
    bool isNumericOnly() const;
    
    void setRightAligned(bool aligned);
    bool isRightAligned() const;
    
    // Properties
    int getMaxCharacters() const { return maxCharacters; }
    bool isEmpty() const { return inputText.empty(); }
};