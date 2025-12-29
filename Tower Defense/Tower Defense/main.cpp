#include <SFML/Graphics.hpp>
#include "mapa.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 1536), "SFML dzia³a!");
    loadMapa();// laduje mape
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        renderMapa(window); //rysuje mape
        window.display();
    }

    return 0;
}
