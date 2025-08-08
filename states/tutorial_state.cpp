#include "states/tutorial_state.hpp"
#include "states/ACT1_state.hpp"
#include "states/start_state.hpp"
#include "game.hpp"
#include "resource_manager.hpp"
#include "audio_manager.hpp"
#include <SFML/Graphics/Rect.hpp>

TutorialState::TutorialState(Game& game) : currentIndex(0) {
    auto& resMgr = ResourceManager::getInstance();
    
    // Load all tutorial textures
    for (const auto& path : tutorialPaths) {
        tutorialTextures.push_back(resMgr.getTexture(path));
    }
    
    // Initialize buttons
    nextButton = std::make_unique<Button>(
        sf::Vector2f(120, 50),
        sf::Vector2f(650, 530),
        sf::Color(0, 150, 200),
        game.getFont(),
        "Next"
    );

    prevButton = std::make_unique<Button>(
        sf::Vector2f(120, 50),
        sf::Vector2f(30, 530),
        sf::Color(200, 100, 0),
        game.getFont(),
        "Back"
    );

    escButton = std::make_unique<Button>(
        sf::Vector2f(150, 50),
        sf::Vector2f(350, 530),
        sf::Color::Green,
        game.getFont(),
        "Esc (Menu)"
    );
    
    proceedButton = std::make_unique<Button>(
        sf::Vector2f(250, 50),
        sf::Vector2f(500, 530),
        sf::Color(0, 50, 0),
        game.getFont(),
        "ACT 1"
    );

    updateButtonVisibility();
}

void TutorialState::updateButtonVisibility() {
    if (currentIndex == 0) {
        nextButton->setVisible(true);
        prevButton->setVisible(false);
        proceedButton->setVisible(false);
    }
    else if (currentIndex == tutorialTextures.size() - 1) {
        nextButton->setVisible(false);
        prevButton->setVisible(true);
        proceedButton->setVisible(true);
    }
    else {
        nextButton->setVisible(true);
        prevButton->setVisible(true);
        proceedButton->setVisible(false);
    }
    
    escButton->setVisible(true);
}

void TutorialState::handleEvents(Game& game) {
    while (auto event = game.getWindow().pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            game.getWindow().close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            auto mousePos = game.getWindow().mapPixelToCoords(
                event->getIf<sf::Event::MouseButtonPressed>()->position);
            lastMousePos = mousePos;
            handleButtonClicks(game, mousePos);
        }
        else if (event->is<sf::Event::MouseMoved>()) {
            lastMousePos = game.getWindow().mapPixelToCoords(
                event->getIf<sf::Event::MouseMoved>()->position);
        }
    }
}

void TutorialState::update(Game& game) {
    if (nextButton && nextButton->isVisible()) nextButton->update(lastMousePos);
    if (prevButton && prevButton->isVisible()) prevButton->update(lastMousePos);
    if (escButton) escButton->update(lastMousePos);
    if (proceedButton && proceedButton->isVisible()) proceedButton->update(lastMousePos);
}

void TutorialState::advancePage() {
    if (currentIndex < tutorialTextures.size() - 1) {
        currentIndex++;
        updateButtonVisibility();
        AudioManager::getInstance().playSound("click");
    }
}

void TutorialState::goBackPage() {
    if (currentIndex > 0) {
        currentIndex--;
        updateButtonVisibility();
        AudioManager::getInstance().playSound("click");
    }
}
void TutorialState::handleButtonClicks(Game& game, const sf::Vector2f& mousePos) {
    // Check next button
    if (nextButton->isVisible() && nextButton->getGlobalBounds().contains(mousePos)) {
        advancePage();
        return;
    }
    
    // Check previous button
    if (prevButton->isVisible() && prevButton->getGlobalBounds().contains(mousePos)) {
        goBackPage();
        return;
    }
    
    // Check proceed to ACT1 button
    if (proceedButton->isVisible() && proceedButton->getGlobalBounds().contains(mousePos)) {
        game.changeState(std::make_unique<ACT1_state>(game.getFont()));
        AudioManager::getInstance().playSound("click");
        return;
    }
    
    // Check menu button
    if (escButton->isVisible() && escButton->getGlobalBounds().contains(mousePos)) {
        game.changeState(std::make_unique<StartState>(game.getFont()));
        AudioManager::getInstance().playSound("click");
        return;
    }
    
    // Optional: Handle click on tutorial image for advance
    if (currentIndex < tutorialTextures.size() - 1 && 
        tutorialTextures[currentIndex].getSize().x > 0) {
        sf::FloatRect imageRect(
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(
                static_cast<float>(tutorialTextures[currentIndex].getSize().x),
                static_cast<float>(tutorialTextures[currentIndex].getSize().y)
            )
        );
        
        if (imageRect.contains(mousePos)) {
            advancePage();
        }
    }
}

void TutorialState::render(Game& game) {
    game.getWindow().clear();
    
    if (currentIndex < tutorialTextures.size()) {
        sf::Sprite currentSprite(tutorialTextures[currentIndex]);
        game.getWindow().draw(currentSprite);
    }
    
    if (nextButton->isVisible()) nextButton->draw(game.getWindow());
    if (prevButton->isVisible()) prevButton->draw(game.getWindow());
    if (proceedButton->isVisible()) proceedButton->draw(game.getWindow());
    if (escButton->isVisible()) escButton->draw(game.getWindow());    
   
    game.getWindow().display();
}