#include "states/tutorial_state.hpp"
#include "game.hpp"

TutorialState::TutorialState(Game& game) 
    : tutorialTextures(game.getTextures()) {
    
    // Initialize buttons
    nextButton = std::make_unique<Button>(
        sf::Vector2f(120, 50),
        sf::Vector2f(650, 530),
        sf::Color(0, 150, 200),
        game.getFont(),
        "Next →"
    );

    prevButton = std::make_unique<Button>(
        sf::Vector2f(120, 50),
        sf::Vector2f(30, 530),
        sf::Color(200, 100, 0),
        game.getFont(),
        "← Back"
    );
}

void TutorialState::handleEvents(Game& game) {
    sf::Event event;
    while (game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = game.getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow()));
            
            if (nextButton->isClicked(mousePos)) {
                game.goToNextPage();
            }
            else if (prevButton->isClicked(mousePos)) {
                game.goToPreviousPage();
            }
        }
    }
}

void TutorialState::render(Game& game) {
    game.getWindow().draw(tutorialTextures[currentIndex]);
    if (nextButton) nextButton->draw(game.getWindow());
    if (prevButton) prevButton->draw(game.getWindow());
}