#include <SFML/Graphics.hpp>
#include "EnemyManager.h"
#include "PathLoader.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Enemy Test");
    window.setFramerateLimit(60);

    EnemyManager manager;

    // Wczytaj punkty z notatnika
    std::vector<sf::Vector2f> path = PathLoader::loadPath("../assets/maps/map1.txt");

    // Przeka¿ punkty do managera

    manager.setPath(path);

    manager.spawnWave(5);

    sf::Clock clock;


    sf::Texture tex;
    tex.loadFromFile("../assets/path.png");

    sf::Sprite sprite(tex);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
            if (e.type == sf::Event::Closed) window.close();

        float dt = clock.restart().asSeconds();

        manager.update(dt);

        window.clear();
        window.draw(sprite);

        manager.draw(window);
        window.display();
    }
}
