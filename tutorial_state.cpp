#include "states/tutorial_state.hpp"
#include "states/ACT1_state.hpp"
#include "states/start_state.hpp"
#include "game.hpp"
#include "resource_manager.hpp"
#include "audio_manager.hpp"
#include "states/start_state.hpp" 

TutorialState::TutorialState(Game& game) : currentIndex(0) {
    auto& resMgr = ResourceManager::getInstance();
    
    // Load tutorial textures efficiently
    tutorialTextures.reserve(tutorialPaths.size());
    for (const auto& path : tutorialPaths) {
        tutorialTextures.push_back(resMgr.getTexture(path));
    }
    
    // Initialize navigation buttons with consistent styling
    nextButton = std::make_unique<Button>(
        sf::Vector2f(120, 45), sf::Vector2f(650, 540),
        sf::Color(50, 150, 80), game.getFont(), "NEXT"
    );

    prevButton = std::make_unique<Button>(
        sf::Vector2f(120, 45), sf::Vector2f(50, 540),
        sf::Color(45, 45, 50), game.getFont(), "BACK"
    );

    escButton = std::make_unique<Button>(
        sf::Vector2f(120, 45), sf::Vector2f(200, 540),
        sf::Color(60, 120, 180), game.getFont(), "MENU"
    );
    
    proceedButton = std::make_unique<Button>(
        sf::Vector2f(180, 45), sf::Vector2f(500, 540),
        sf::Color(150, 80, 200), game.getFont(), "START ACT 1"
    );

    updateButtonVisibility();
}

void TutorialState::updateButtonVisibility() {
    const bool isFirst = (currentIndex == 0);
    const bool isLast = (currentIndex == tutorialTextures.size() - 1);
    
    nextButton->setVisible(!isLast);
    prevButton->setVisible(!isFirst);
    proceedButton->setVisible(isLast);
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
        else if (event->is<sf::Event::KeyPressed>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
            
            if (keyEvent->code == sf::Keyboard::Key::Right && currentIndex < tutorialTextures.size() - 1) {
                advancePage();
            }
            else if (keyEvent->code == sf::Keyboard::Key::Left && currentIndex > 0) {
                goBackPage();
            }
            else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                game.changeState(std::make_unique<StartState>(game.getFont()));
                AudioManager::getInstance().playSound("click");
            }
        }
    }
}

void TutorialState::update(Game& game) {
    // Update button hover states efficiently
    nextButton->update(lastMousePos);
    prevButton->update(lastMousePos);
    escButton->update(lastMousePos);
    proceedButton->update(lastMousePos);
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
    if (nextButton->isVisible() && nextButton->getGlobalBounds().contains(mousePos)) {
        advancePage();
    }
    else if (prevButton->isVisible() && prevButton->getGlobalBounds().contains(mousePos)) {
        goBackPage();
    }
    else if (proceedButton->isVisible() && proceedButton->getGlobalBounds().contains(mousePos)) {
        game.changeState(std::make_unique<ACT1_state>(game.getFont()));
        AudioManager::getInstance().playSound("click");
    }
    else if (escButton->isVisible() && escButton->getGlobalBounds().contains(mousePos)) {
        game.changeState(std::make_unique<StartState>(game.getFont()));
        AudioManager::getInstance().playSound("click");
    }
    else if (currentIndex < tutorialTextures.size() - 1 && tutorialTextures[currentIndex].getSize().x > 0) {
        // Silent click anywhere on tutorial image to advance
        sf::FloatRect imageRect(sf::Vector2f(0.f, 0.f), 
            sf::Vector2f(static_cast<float>(tutorialTextures[currentIndex].getSize().x),
                         static_cast<float>(tutorialTextures[currentIndex].getSize().y)));
        
        if (imageRect.contains(mousePos)) {
            advancePage();
        }
    }
}

void TutorialState::render(Game& game) {
    game.getWindow().clear(sf::Color(20, 25, 35));
    
    // Draw current tutorial image
    if (currentIndex < tutorialTextures.size()) {
        sf::Sprite currentSprite(tutorialTextures[currentIndex]);
        
        // Scale and center efficiently
        sf::Vector2u windowSize = game.getWindow().getSize();
        sf::Vector2u textureSize = tutorialTextures[currentIndex].getSize();
        
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y - 80) / textureSize.y;
        float scale = std::min(scaleX, scaleY);
        
        currentSprite.setScale(sf::Vector2f(scale, scale));
        currentSprite.setOrigin(sf::Vector2f(textureSize.x/2.f, textureSize.y/2.f));
        currentSprite.setPosition(sf::Vector2f(windowSize.x/2.f, (windowSize.y - 80)/2.f));
        
        game.getWindow().draw(currentSprite);
    }
    
    // Draw UI bar
    sf::RectangleShape uiBar(sf::Vector2f(800, 70));
    uiBar.setPosition(sf::Vector2f(0, 530));
    uiBar.setFillColor(sf::Color(15, 20, 30, 200));
    game.getWindow().draw(uiBar);
    
    // Draw page indicator
    sf::Text pageIndicator(game.getFont());
    pageIndicator.setString("Tutorial: " + std::to_string(currentIndex + 1) + " / " + std::to_string(tutorialTextures.size()));
    pageIndicator.setCharacterSize(18);
    pageIndicator.setFillColor(sf::Color(200, 200, 200));
    pageIndicator.setPosition(sf::Vector2f(350, 550));
    game.getWindow().draw(pageIndicator);
    
    // Draw buttons
    if (nextButton->isVisible()) nextButton->draw(game.getWindow());
    if (prevButton->isVisible()) prevButton->draw(game.getWindow());
    if (proceedButton->isVisible()) proceedButton->draw(game.getWindow());
    if (escButton->isVisible()) escButton->draw(game.getWindow());    
   
    game.getWindow().display();
}