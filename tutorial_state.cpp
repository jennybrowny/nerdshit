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
    while (auto event = game.getWindow().pollEvent()) {
        if (event->is<sf::Event::MouseButtonPressed>()) {
            const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            auto mousePos = game.getWindow().mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});

            if (nextButton->isClicked(mousePos)) {
                game.goToNextPage();
            }
            else if (prevButton->isClicked(mousePos)) {
                game.goToPreviousPage();
            }
        }
    }
}

void TutorialState::update(Game& game) {
    // Empty for now
}

void TutorialState::render(Game& game) {
    game.getWindow().clear();

    // Draw current tutorial image
    if (game.getCurrentTutorialSprite()) {
        game.getWindow().draw(*game.getCurrentTutorialSprite());
    }

    // Draw buttons
    if (nextButton) nextButton->draw(game.getWindow());
    if (prevButton) prevButton->draw(game.getWindow());

    game.getWindow().display();
}