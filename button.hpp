#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <SFML/Window/Event.hpp>

// Interface for button behaviors (Strategy Pattern)
// This allows different button types to implement different behaviors
class IButtonBehavior {
public:
    virtual ~IButtonBehavior() = default;
    virtual void onClick() = 0;    // Pure virtual click handler
    virtual void onHover() = 0;    // Pure virtual hover handler
};

// Handles visual representation (Composite Pattern)
// Encapsulates all visual aspects of a button
class ButtonVisual {
protected:
    sf::RectangleShape shape;      // Base button rectangle
    std::unique_ptr<sf::Text> text; // Smart pointer to label text
    sf::Color normalColor;         // Default color
    sf::Color hoverColor;          // Mouse-over color
    
public:
    // Constructor handles all visual setup
    ButtonVisual(sf::Vector2f size, sf::Vector2f position, 
                const sf::Color& color, const sf::Font& font,
                const std::string& label);
    
    // Drawing uses SFML's drawable interface
    void draw(sf::RenderWindow& window) const;
    
    // State management
    void setHoverState(bool isHovered);
    
    // Hit detection using SFML's bounds checking
    bool contains(sf::Vector2f point) const;
};

// Handles button sounds (Singleton-like pattern)
// Manages shared sound resources across all buttons
class SoundManager {
    static sf::SoundBuffer clickBuffer; // Shared sound resource
    static bool soundLoaded;            // Resource loading flag
    sf::Sound clickSound;               // Instance-specific sound
    
public:
    SoundManager();              // Ensures sound is loaded
    void playClick(float volume = 100.f); // Plays click sound
};

// Main button class (Multiple Inheritance)
// Combines behavior, visuals, and sound
class Button : public IButtonBehavior, private ButtonVisual, private SoundManager {
public:
    // Initializes all components
    Button(sf::Vector2f size, sf::Vector2f position, 
          const sf::Color& color, const sf::Font& font,
          const std::string& text);
    
    // Interface implementations
    void onClick() override;  // Handles click behavior
    void onHover() override;  // Handles hover behavior
    
    // Public drawing interface
    void draw(sf::RenderWindow& window) const;
    
    // SFML event processing
    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);
};