#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "EnemyManager.h"
#include "PathLoader.h"
#include "mapa.h"
#include "hud.h"
#include "forge.h"

// !!!
//
//enemymanager.cpp -> statysyki przeciwnika
//ememymanager.h -> zycie i kasa gracza


WaveConfig getWaveSettings(int waveNumber); //funkcja do ustawiania poziomu fal (napisana na dole)

int main() {
    // Konfiguracja okna wyœwietlania i limitu klatek na sekundê
    sf::RenderWindow window(sf::VideoMode(1536, 1024), "Tower Defense - SFML dzia³a!");
    window.setFramerateLimit(60);

    // Inicjalizacja managera przeciwników(bardzo wazne klasa dla gracza i przeciwknika, duzo sie na niej opiera logiki)
    EnemyManager manager;

    // Wczytanie wspolrzedych mapy i przekazanie ich do managera
    std::vector<sf::Vector2f> path = PathLoader::loadPath("../Assets/maps/map1.txt");
    manager.setPath(path);

    // Inicjalizacja elementów sceny
    initForge(window); //inicjalizuje kuznie
    initHUD(); // inicjalizuje HUD
    loadMapa(); // laduje mape

    // Za³adowanie tekstur menu i game over 
    sf::Texture menuTexture; //tekstura przechowujaca grafike menu
    sf::Sprite menuSprite; //sprite odpowiedzialny za rysowanie grafiki
    menuTexture.loadFromFile("../Assets/menu/menu.png"); //ladujemy grafike menu
    menuSprite.setTexture(menuTexture); //ustawiamy teksture dla sprite'a
    menuSprite.setPosition(0.f, 0.f); //ustawiamy pozycje sprite na (0,0)

    sf::Texture gameOverTexture; //tekstura przechowujaca grafike game over
    sf::Sprite gameOverSprite; //sprite odpowiedzialny za rysowanie grafiki
    gameOverTexture.loadFromFile("../Assets/gameover/gameover.png"); //ladujemy grafike menu
    gameOverSprite.setTexture(gameOverTexture); //ustawiamy teksture dla sprite'a
    gameOverSprite.setPosition(0.f, 0.f); //ustawiamy pozycje sprite na (0,0)

    sf::FloatRect przyciskStart(450.f, 300.f, 300.f, 80.f);

    // Zmienne steruj¹ce stanem gry
    sf::Clock clock;
    int currentWave = 0;
    bool graStart = false; //zmienna sprawdzajaca czy gra zostala ropoczeta
    bool gamePaused = false;

    // G³ówna pêtla gry
    while (window.isOpen()) {
        sf::Event event;
        float dt = clock.restart().asSeconds();

        // Obs³uga zdarzeñ systemowych (zamkniêcie okna, sterowanie)
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            handleForgeEvent(event); //obsluga kuzni

            // Obs³uga klawisza Escape
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                graStart = false;
                // gameOver = false; // Logika gameOver jest teraz pobierana z managera
            }

            // Menu przed rozpoczêciem gry
            if (!graStart) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (przyciskStart.contains(mousePos)) {
                        graStart = true; //ustawiamy zmienna na true, aby rozpocz gre
                    }
                }
            }
            // Logika w trakcie gry
            else if (!manager.gameOver()) {
                // do testu, zadanie obrazen przyciskiem myszy
                // tu bedzie zmiana na wieze
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // pozycja myszy wzglêdem okna
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    // funkcja zadawania obra¿eñ MYSZY!!!
                    manager.MouseClick(worldPos);
                }

                // Uruchomienie nowej fali po naciœniêciu Spacji, jeœli obecna fala siê zakoñczy³a
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                    if (!manager.isWaveActive()) {
                        currentWave++;

                        // ustawienia sa zalezne od fali
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
        }

        //aktualizacja gry gdy ta nie jest zatrzymana
        if (graStart && !gamePaused && !manager.gameOver()) {
            manager.update(dt);
        }

        // Rysowanie sceny
        window.clear();

        if (!graStart) {
            window.draw(menuSprite); //rysuje menu
        }
        else if (manager.gameOver()) {
            window.draw(gameOverSprite); // Wyœwietlenie ekranu koñca gry
        }
        else {
            // Rysowanie aktywnej rozgrywki
            renderMapa(window); //rysuje mape
            manager.draw(window); // Rysowanie przeciwników i logiki
            rysujHUD(window); //rysuje hud 
            rysujForge(window); //rysuje kuznie 
        }

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