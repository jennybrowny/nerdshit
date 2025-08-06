#include "states/credits_state.hpp"
#include "game.hpp"
#include "states/start_state.hpp"
#include <iostream>
#include <vector>
#include <cmath>

CreditsState::CreditsState(sf::Font& font) : 
    creditsText(font, "", 30),
    backgroundTexture(),
    backgroundSprite(backgroundTexture),
    rainbow(sf::PrimitiveType::TriangleStrip),
    gradientPosition(0.f)
{
    auto& audio = AudioManager::getInstance();
    audio.loadMusic("credits", "assets/sounds/credits.mp3");
    
    rainbowColors = {
        sf::Color(255, 0, 0), sf::Color(255, 127, 0),
        sf::Color(255, 255, 0), sf::Color(0, 255, 0),
        sf::Color(0, 0, 255), sf::Color(75, 0, 130),
        sf::Color(148, 0, 211)
    };
    
    createRainbowBackground();
    
    backButton = std::make_unique<Button>(
        sf::Vector2f(200, 60),
        sf::Vector2f(300, 500),
        sf::Color::Red,
        font,
        "BACK"
    );
    
    creditsText.setString(
        "CREDITS\n\n"
        "Created by: Jenny Brown\n"
        "Music by: Illegal Pirating\n"
        "Special Thanks to:\n My soulmate, Chatgpt,\n Deepseek,\n Mangoman,\n Dr. Herve Collins\n\n"
        "I believe - Dr. Paul Schmitt Ph.D.\n"
    );
    creditsText.setFillColor(sf::Color::White);
}

void CreditsState::update(Game& game) {
    updateRainbow();
}

void CreditsState::createRainbowBackground() {
    const int segments = 50;
    rainbow.clear();
    rainbow.resize(segments * 2);
    
    for (int i = 0; i < segments; i++) {
        float x = 800.f * i / (segments - 1);
        rainbow[i*2].position = sf::Vector2f(x, 0);
        rainbow[i*2+1].position = sf::Vector2f(x, 600);
    }
}

void CreditsState::updateRainbow() {
    gradientPosition += 0.0001f;
    gradientPosition = std::fmod(gradientPosition, 1.f);
    
    for (size_t i = 0; i < rainbow.getVertexCount(); i++) {
        float pos = gradientPosition + (0.2f * i / rainbow.getVertexCount());
        pos = std::fmod(pos, 1.f);
        
        int colorIndex = static_cast<int>(pos * rainbowColors.size()) % rainbowColors.size();
        int nextColorIndex = (colorIndex + 1) % rainbowColors.size();
        float blend = (pos * rainbowColors.size()) - colorIndex;
        
        sf::Color blendedColor(
            rainbowColors[colorIndex].r + (rainbowColors[nextColorIndex].r - rainbowColors[colorIndex].r) * blend,
            rainbowColors[colorIndex].g + (rainbowColors[nextColorIndex].g - rainbowColors[colorIndex].g) * blend,
            rainbowColors[colorIndex].b + (rainbowColors[nextColorIndex].b - rainbowColors[colorIndex].b) * blend
        );
        
        rainbow[i].color = blendedColor;
    }
}

void CreditsState::handleEvents(Game& game) {
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow()));
    
    while (auto event = game.getWindow().pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            game.getWindow().close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            if (backButton && backButton->isClicked(mousePos)) {
                AudioManager::getInstance().playMusic("credits");
                game.changeState(std::make_unique<StartState>(game.getFont()));
            }
        }
    }
}

void CreditsState::render(Game& game) {
    game.getWindow().clear();
    game.getWindow().draw(rainbow);
    game.getWindow().draw(creditsText);
    if (backButton) {
        backButton->draw(game.getWindow());
    }
    game.getWindow().display();
}