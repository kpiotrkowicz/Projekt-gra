//      !!!
//
//enemymanager.cpp -> statysyki przeciwnika
//ememymanager.h -> zycie i kasa gracza

#include <SFML/Graphics.hpp>
#include <iostream>
#include "EnemyManager.h"
#include "PathLoader.h" 
#include "mapa.h"
#include "hud.h"
#include "forge.h"

bool graStart = false;//zmienna sprawdzajaca czy gra zostala ropoczeta
bool gameOver = false;
int wynik = 0; //zmienna przechowujaca wynik

int main() {
    //sf::Font font;
    //font.loadFromFile("../Assets/fonts/arial.ttf"); //ladujemy czcionke)
    
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
    //wynik na tle gameover
    /*sf::Text tekstWynik;
    tekstWynik.setFont(font);
    tekstWynik.setCharacterSize(40);
    tekstWynik.setFillColor(sf::Color::White);
    tekstWynik.setPosition(500.f, 400.f);*/

    sf::RenderWindow window(sf::VideoMode(1536, 1024), "SFML dzia³a!");
    initForge(window);//inicjalizuje kuznie
    initHUD(); // inicjalizuje HUD
    loadMapa();// laduje mape
    
    while (window.isOpen()) {
        sf::Event event;
/// <summary>
/// ///
/// </summary>
WaveConfig getWaveSettings(int); //funkcja do ustawiania poziomu fal (napisana na dole)


    // Konfiguracja okna wyœwietlania i limitu klatek na sekundê
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense");
    window.setFramerateLimit(60);

    EnemyManager manager;

    // Wczytanie wspolrzedych mapy i przekazanie ich do managera
    std::vector<sf::Vector2f> path = PathLoader::loadPath("../Assets/maps/map1.txt");
    manager.setPath(path);

	// Za³adowanie t³a mapy i tla konca gry
    sf::Texture tex;
    if (!tex.loadFromFile("../Assets/path.png")) { /* obs³uga b³êdu braku pliku */ }
    sf::Sprite sprite(tex);

    sf::Texture gameOverTex;
	if (!gameOverTex.loadFromFile("../Assets/koniec.png")) { /* obs³uga b³êdu braku pliku */ }
	sf::Sprite gameOverSprite(gameOverTex);
   

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

            //do testu, zadanie obrazen przyciskiem myszy
            //tu bedzie zmiana na wieze
            if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    //pozycja myszy wzglêdem okna
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    //funkcja zadawania obra¿eñ
                    manager.MouseClick(worldPos);
                }
            }
            if (e.type == sf::Event::Closed)
                window.close();


            if (!manager.gameOver()) {
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
        }

        // Do aktualizacji gry
        float dt = clock.restart().asSeconds();

        // Aktualizacja gry gdy aplikacja nie jest zatrzymana
        if (!gamePaused && !manager.gameOver()) {
            manager.update(dt);
            handleForgeEvent(event);//obsluga kuzni
            
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                graStart = false;
                gameOver = false;
                
            }
            if (!graStart && event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(
                    event.mouseButton.x,
                    event.mouseButton.y
                );
                if (przyciskStart.contains(mousePos)) {
                    graStart = true; //ustawiamy zmienna na true, aby rozpocz¹æ gre
                }
            }
        }
       
        window.clear();
        if (!graStart) {
        window.draw(menuSprite); //rysuje menu
        }
        else if (gameOver) {
            window.draw(gameOverSprite);
        }
        else {
			renderMapa(window); //rysuje mape
            rysujHUD(window);//rysuje hud
            rysujForge(window);//rysuje kuznie
        }

        // Rysowanie sceny
        window.clear();
        window.draw(sprite);   // Wyœwietlenie t³a
        manager.draw(window);
        if (manager.gameOver()) { 
			window.clear(); //usuwanie tla i potworow
            window.draw(gameOverSprite); // Wyœwietlenie ekranu koñca gry
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