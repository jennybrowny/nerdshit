#include "../game.hpp" 

StartState::StartState(sf::Fontx& font) {
    startButton = std::make_unique<Button>(
        sf::Vector2f(200, 80),
        sf::Vector2f(300, 260),
        sf::Color::Yellow,
        font,
        "START"
    );
}

void StartState::handleEvents(Game& game) {
    sf::Event event;
    while (game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = game.getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow()));
            if (startButton->isClicked(mousePos)) {
                AudioManager::getInstance().playSound("click");
                game.changeState(std::make_unique<TutorialState>(game));
            }
        }
    }
}

void StartState::render(Game& game) {
    if (game.getTitleSprite()) {
        game.getWindow().draw(*game.getTitleSprite());
    }
    startButton->draw(game.getWindow());
}