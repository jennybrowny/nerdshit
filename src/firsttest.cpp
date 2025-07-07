#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "First SFML Test");
    sf::RectangleShape square(sf::Vector2f(50.f, 50.f));
    square.setFillColor(sf::Color::Red);
    float x = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        // Update position
        x += 0.5;  // adjust speed as needed
        if (x > 500) x = 0;
        square.setPosition(x, 225);

        window.clear();
        window.draw(square);
        window.display();
    }

    return 0;
}