#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 400, 300 }), "Teste SFML");

    while (window.isOpen()) {
        std::optional<sf::Event> event;
        while (event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
