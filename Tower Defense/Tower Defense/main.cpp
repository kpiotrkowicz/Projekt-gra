#include <SFML/Graphics.hpp>
#include "EnemyManager.h"
#include "PathLoader.h"
#include <iostream>

int main()
{
    // Konfiguracja okna wyœwietlania i limitu klatek na sekundê
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense");
    window.setFramerateLimit(60);

    EnemyManager manager;

    // Wczytanie wspolrzedych mapy i przekazanie ich do managera
    std::vector<sf::Vector2f> path = PathLoader::loadPath("../assets/maps/map1.txt");
    manager.setPath(path);

    // Za³adowanie t³a mapy
    sf::Texture tex;
    if (!tex.loadFromFile("../assets/path.png")) { /* obs³uga b³êdu braku pliku */ }
    sf::Sprite sprite(tex);

    sf::Clock clock;
    int currentWave = 0;
    bool gamePaused = false;

    // G³ówna pêtla gry
    while (window.isOpen())
    {
        sf::Event e;
        // Obs³uga zdarzeñ systemowych (zamkniêcie okna, sterowanie)
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            // Uruchomienie nowej fali po naciœniêciu Spacji, jeœli obecna fala siê zakoñczy³a
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space) {
                if (!manager.isWaveActive()) {
                    currentWave++;

					// Trudnosc zalezy od numeru rundy
                    int typeToSpawn = 0;
                    if (currentWave > 2) typeToSpawn = 1;
                    if (currentWave > 5) typeToSpawn = 2;

                    manager.startWave(currentWave * 5, 0.8f, typeToSpawn);

                    // Wyœwietlenie informacji o nowej rundzie w konsoli
                    std::system("cls");
                    std::cout << "===========================" << std::endl;
                    std::cout << "RUNDA: " << currentWave << std::endl;
                    std::cout << "TYP PRZECIWNIKA: " << typeToSpawn << std::endl;
                    std::cout << "===========================" << std::endl;
                }
            }
        }

        // Do aktualizacji gry
        float dt = clock.restart().asSeconds();

        // Aktualizacja gry gdy aplikacja nie jest zatrzymana
        if (!gamePaused) {
            manager.update(dt);
        }

        // Rysowanie sceny
        window.clear();
        window.draw(sprite);   // Wyœwietlenie t³a
        manager.draw(window); 
        window.display();
    }

    return 0;
}