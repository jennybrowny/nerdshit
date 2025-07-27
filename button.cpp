#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <iostream>

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

    std::function<void()> onClick; // Click callback
};

/*
Summary of debugging 7/27 7pm - 7:35pm
✅ Fixed Constructor Syntax
✅ Proper Sound Initialization
- they were in the member class cause Iʻm a stupid bitch
✅ Robust Error Handling
✅ Precise Text Centering
✅ Optimized Event Detection
✅ SFML 2.x/3.x Compatibility
- AI does not know 2.5 vs 3.0 version 
✅ Clean OOP Structure
✅ Fallbacks
- itʻs so fucking stupid how like you have to tell the computer this is a button, this is a click put them together and WOW
*/