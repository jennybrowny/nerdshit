#include "input_textbox.hpp"
#include "audio_manager.hpp"
#include <algorithm>
#include <cstdint>  

InputTextBox::InputTextBox(const sf::Vector2f& size, const sf::Vector2f& position, 
                           sf::Font& font, int maxCharacters)
    : font(font), maxCharacters(maxCharacters), isFocused(false), isVisible(true),
      cursorBlinkTimer(0.0f), showCursor(true), displayText(font), placeholderDisplay(font),
      numericOnly(false), rightAligned(false)  // NEW: Initialize new properties
{
    // Set up background
    background.setSize(size);
    background.setPosition(position);
    background.setOutlineThickness(2);
    
    // Colors
    normalColor = sf::Color(40, 50, 60, 200);
    focusedColor = sf::Color(60, 80, 120, 220);
    textColor = sf::Color::White;
    placeholderColor = sf::Color(120, 120, 120);
    
    background.setFillColor(normalColor);
    background.setOutlineColor(sf::Color(80, 100, 140));
    
    // Set up cursor
    cursor.setSize(sf::Vector2f(2, size.y - 8));
    cursor.setFillColor(sf::Color::White);
    
    // Set up text
    displayText.setCharacterSize(18);
    displayText.setFillColor(textColor);
    displayText.setPosition(sf::Vector2f(position.x + 8, position.y + 8));
    
    placeholderDisplay.setCharacterSize(18);
    placeholderDisplay.setFillColor(placeholderColor);
    placeholderDisplay.setPosition(sf::Vector2f(position.x + 8, position.y + 8));
    
    placeholderText = "Enter text...";
    placeholderDisplay.setString(placeholderText);
    
    updateDisplayText();
}

void InputTextBox::handleEvent(const sf::Event& event) {
    if (!isVisible) return;
    
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x), 
                              static_cast<float>(mouseEvent->position.y));
        
        // Check if clicked inside the text box
        bool wasClicked = background.getGlobalBounds().contains(mousePos);
        setFocused(wasClicked);
        
        if (wasClicked) {
            try {
                AudioManager::getInstance().playSound("click");
            } catch (...) {
                // Ignore audio errors
            }
        }
    }
    else if (event.is<sf::Event::TextEntered>() && isFocused) {
        const auto& textEvent = event.getIf<sf::Event::TextEntered>();
        handleTextEntered(textEvent->unicode);
    }
    else if (event.is<sf::Event::KeyPressed>() && isFocused) {
        const auto& keyEvent = event.getIf<sf::Event::KeyPressed>();
        
        if (keyEvent->code == sf::Keyboard::Key::Backspace && !inputText.empty()) {
            inputText.pop_back();
            updateDisplayText();
            
            try {
                AudioManager::getInstance().playSound("click");
            } catch (...) {
                // Ignore audio errors
            }
        }
        else if (keyEvent->code == sf::Keyboard::Key::Delete) {
            inputText.clear();
            updateDisplayText();
        }
    }
}

void InputTextBox::handleTextEntered(std::uint32_t unicode) {
    // NEW: Filter for numeric-only mode
    if (numericOnly) {
        // Only allow digits 0-9
        if (unicode >= '0' && unicode <= '9' && static_cast<int>(inputText.length()) < maxCharacters) {
            inputText += static_cast<char>(unicode);
            updateDisplayText();
            
            try {
                AudioManager::getInstance().playSound("click");
            } catch (...) {
                // Ignore audio errors
            }
        }
    } else {
        // Original behavior: Filter out control characters
        if (unicode >= 32 && unicode < 127 && static_cast<int>(inputText.length()) < maxCharacters) {
            inputText += static_cast<char>(unicode);
            updateDisplayText();
            
            try {
                AudioManager::getInstance().playSound("click");
            } catch (...) {
                // Ignore audio errors
            }
        }
    }
}

void InputTextBox::update(float deltaTime) {
    if (!isVisible) return;
    
    // Update cursor blinking
    cursorBlinkTimer += deltaTime;
    if (cursorBlinkTimer >= 0.5f) {
        showCursor = !showCursor;
        cursorBlinkTimer = 0.0f;
    }
    
    // Update background color based on focus
    if (isFocused) {
        background.setFillColor(focusedColor);
        background.setOutlineColor(sf::Color(120, 160, 255));
    } else {
        background.setFillColor(normalColor);
        background.setOutlineColor(sf::Color(80, 100, 140));
    }
    
    // Update cursor position - ENHANCED for right alignment
    if (isFocused && !inputText.empty()) {
        sf::Vector2f textPos = displayText.getPosition();
        sf::FloatRect textBounds = displayText.getLocalBounds();
        
        if (rightAligned) {
            // Position cursor at the right edge of the text
            cursor.setPosition(sf::Vector2f(textPos.x + textBounds.size.x + 2, textPos.y));
        } else {
            // Original behavior: cursor at end of text
            cursor.setPosition(sf::Vector2f(textPos.x + textBounds.size.x + 2, textPos.y));
        }
    }
}

void InputTextBox::draw(sf::RenderWindow& window) {
    if (!isVisible) return;
    
    // Draw background
    window.draw(background);
    
    // Draw text or placeholder
    if (inputText.empty() && !isFocused) {
        window.draw(placeholderDisplay);
    } else {
        window.draw(displayText);
        
        // Draw cursor if focused
        if (isFocused && showCursor) {
            window.draw(cursor);
        }
    }
}

void InputTextBox::setText(const std::string& text) {
    // NEW: Filter text based on numeric-only mode
    if (numericOnly) {
        std::string filteredText;
        for (char c : text) {
            if (c >= '0' && c <= '9' && static_cast<int>(filteredText.length()) < maxCharacters) {
                filteredText += c;
            }
        }
        inputText = filteredText;
    } else {
        inputText = text.substr(0, std::min(static_cast<int>(text.length()), maxCharacters));
    }
    updateDisplayText();
}

void InputTextBox::setPosition(const sf::Vector2f& pos) {
    background.setPosition(pos);
    
    // Update text positions based on alignment
    if (rightAligned) {
        // Right-aligned text positioning
        sf::Vector2f backgroundSize = background.getSize();
        displayText.setPosition(sf::Vector2f(pos.x + backgroundSize.x - 8, pos.y + 8));
        placeholderDisplay.setPosition(sf::Vector2f(pos.x + backgroundSize.x - 8, pos.y + 8));
    } else {
        // Left-aligned (original behavior)
        displayText.setPosition(sf::Vector2f(pos.x + 8, pos.y + 8));
        placeholderDisplay.setPosition(sf::Vector2f(pos.x + 8, pos.y + 8));
    }
}

sf::FloatRect InputTextBox::getGlobalBounds() const {
    return background.getGlobalBounds();
}

void InputTextBox::setPlaceholderText(const std::string& placeholder) {
    placeholderText = placeholder;
    placeholderDisplay.setString(placeholderText);
    
    // Update placeholder positioning if right-aligned
    if (rightAligned) {
        sf::Vector2f backgroundPos = background.getPosition();
        sf::Vector2f backgroundSize = background.getSize();
        sf::FloatRect placeholderBounds = placeholderDisplay.getLocalBounds();
        
        placeholderDisplay.setOrigin(sf::Vector2f(
            placeholderBounds.position.x + placeholderBounds.size.x,
            placeholderBounds.position.y
        ));
        placeholderDisplay.setPosition(sf::Vector2f(backgroundPos.x + backgroundSize.x - 8, backgroundPos.y + 8));
    }
}

// NEW: Set numeric-only mode
void InputTextBox::setNumericOnly(bool numeric) {
    numericOnly = numeric;
    if (numeric) {
        // Filter existing text to only contain digits
        std::string filteredText;
        for (char c : inputText) {
            if (c >= '0' && c <= '9') {
                filteredText += c;
            }
        }
        inputText = filteredText;
        updateDisplayText();
    }
}

// NEW: Set right-aligned text mode
void InputTextBox::setRightAligned(bool aligned) {
    rightAligned = aligned;
    updateDisplayText();
    
    // Update text positions immediately
    sf::Vector2f currentPos = background.getPosition();
    setPosition(currentPos);
}

// NEW: Get numeric-only mode status
bool InputTextBox::isNumericOnly() const {
    return numericOnly;
}

// NEW: Get right-aligned mode status  
bool InputTextBox::isRightAligned() const {
    return rightAligned;
}

// NEW: Get focused status (for external checking)
bool InputTextBox::isFocusedBox() const {
    return isFocused;
}

void InputTextBox::updateDisplayText() {
    displayText.setString(inputText);
    
    // NEW: Handle right alignment
    if (rightAligned && !inputText.empty()) {
        sf::Vector2f backgroundPos = background.getPosition();
        sf::Vector2f backgroundSize = background.getSize();
        sf::FloatRect textBounds = displayText.getLocalBounds();
        
        // Set origin to right edge of text
        displayText.setOrigin(sf::Vector2f(
            textBounds.position.x + textBounds.size.x,
            textBounds.position.y
        ));
        displayText.setPosition(sf::Vector2f(backgroundPos.x + backgroundSize.x - 8, backgroundPos.y + 8));
    } else if (!rightAligned) {
        // Reset origin for left alignment
        displayText.setOrigin(sf::Vector2f(0, 0));
        sf::Vector2f backgroundPos = background.getPosition();
        displayText.setPosition(sf::Vector2f(backgroundPos.x + 8, backgroundPos.y + 8));
    }
}