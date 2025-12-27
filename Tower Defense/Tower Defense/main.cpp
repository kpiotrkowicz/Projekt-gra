//srawdzic poprawnosc i napisac kometarze 
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include "EnemyManager.h"
#include "PathLoader.h" 

WaveConfig getWaveSettings(int); //funkcja do ustawiania poziomu fal (napisana na dole)

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

                    //ustawienia sa zalezne od fali
                    WaveConfig settings = getWaveSettings(currentWave);
                    manager.startWave(settings);

                    // Wyœwietlenie informacji o nowej rundzie w konsoli
                    std::system("cls");
                    std::cout << "===========================" << std::endl;
                    std::cout << "RUNDA: " << currentWave << std::endl;
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

WaveConfig getWaveSettings(int waveNumber) {//scenariusze fali
    WaveConfig config;
    config.delay = std::max(0.3f, 1.0f - (waveNumber * 0.05f));


    int totalEnemies = 5 + (waveNumber * 2);
    

    for (int i = 0; i < totalEnemies; i++) {
        if (i % 3 == 0 && waveNumber >= 3) { //co trzeci przeciwnik to typ 1 od rundy 3
            config.kolejnoscEnemies.push_back(1);
        }
        else if (i % 4 == 0 && waveNumber >= 3) { // runda 4, 8, 15 itd 
            //niby mozna samego if ale chyba tworzy sie za duzo przeciwnikow
            config.kolejnoscEnemies.push_back(2);
        }
        else {
            config.kolejnoscEnemies.push_back(0); //reszta to typ standardowy
        }
    }

    //system bossa
    // Na koniec fali co 5 rund dodaj bossa tutaj typ 2
    //tylko tworza sie teraz tez inne potwory
    if (waveNumber % 5 == 0) {
        config.kolejnoscEnemies.push_back(2);
    }

    return config;
}