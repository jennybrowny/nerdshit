#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class BaseButton {
protected:
    static sf::SoundBuffer clickBuffer;
    static bool soundLoaded;
    sf::Sound clickSound;

    // Private helper to initialize sound
    void initSound() {
        if (!soundLoaded) {
            if (!clickBuffer.loadFromFile("assets/sounds/button_click.mp3")) {
                std::cerr << "Failed to load button click sound!" << std::endl;
            }
            soundLoaded = true;
        }
        clickSound.setBuffer(clickBuffer);
    }

public:
    BaseButton() : clickSound(clickBuffer) {  // Initialize with buffer (even if empty)
        initSound();  // Then properly load and set the buffer
    }
    
    virtual ~BaseButton() = default;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool isClicked(sf::Vector2f mousePos) = 0;

protected:
    void playClickSound(float volume = 100.f) {
        clickSound.setVolume(volume);
        clickSound.play();
    }
};

// Static member initialization
sf::SoundBuffer BaseButton::clickBuffer;
bool BaseButton::soundLoaded = false;

class Button : public BaseButton {
public:
    Button(sf::Vector2f size, sf::Vector2f position, sf::Color color, 
           sf::Font& font, const std::string& text);
    ~Button();

    void draw(sf::RenderWindow& window) override;
    bool isClicked(sf::Vector2f mousePos) override;

private:
    sf::RectangleShape buttonShape;
    sf::Text* buttonText;
    sf::Color buttonColor;
};