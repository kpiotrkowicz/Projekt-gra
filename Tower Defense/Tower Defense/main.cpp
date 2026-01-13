#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "KierownikWiezy.h"
#include <functional>//dla funtion
#include "zabojcacelow.h"
#include "interfejs.h"
#include <map>
#include <algorithm>
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


//wskaznik globalny do kierownika wrogow aby bylo mozna z niego korzystac w callbacku
EnemyManager* g_enemyManager = nullptr;
KierownikWiezy* g_kierownikWiezy = nullptr;

//funkcja ktora wywola kierownik wiezy gdy pocisk trafi w cel
static void PrzyznajObrazenia(int celId, float ilosobrazen) {
    if (g_enemyManager) {
		//funkcja znajduje sie w EmenyManager.cpp
        g_enemyManager->damageEnemy(celId, ilosobrazen);
    }
    /*else {
        std::cout << "Cel o ID: " << celId << " nie istnieje!" << endl;
    }*/
};

//tworzenie celow - funkcja wywolywana przez wieze przy strzale
static vector<Cel>StworzListeCelow(EnemyManager& manager) {
    vector<Cel>lista_celow;

	//tez jest to przezucone do EnemyManager.cpp

    auto& wrogowie = manager.getActiveEnemies();
    for (const auto& wrog : wrogowie) {
        lista_celow.push_back({ wrog.getID(), wrog.getPosition(), (float)wrog.getHp()});
    }

    
    return lista_celow;
};


//tworzymy pocisk
static void Stworzpocisk(int wiezaId, int celId, sf::Vector2f pozycjaStartowa, float obrazenia) {
    if (g_kierownikWiezy) {
        //funkcja znajduje sie w KierownikWiezy.cpp
        g_kierownikWiezy->UtworzPocisk(wiezaId, celId, pozycjaStartowa, obrazenia);
    }
}


WaveConfig getWaveSettings(int waveNumber); //funkcja do ustawiania poziomu fal (napisana na dole)


int main() {


    
    // Konfiguracja okna wyœwietlania i limitu klatek na sekundê 
    sf::RenderWindow window(sf::VideoMode(1536, 1024), "Tower Defense !");
    window.setFramerateLimit(60);
    sf::View worldView = window.getDefaultView();
    sf::View uiView = window.getDefaultView();

    



    // Inicjalizacja managera przeciwników(bardzo wazne klasa dla gracza i przeciwknika, duzo sie na niej opiera logiki
    EnemyManager manager;
    g_enemyManager = &manager; // Przypisanie do wskaŸnika globalnego dla callbacku

    // Wczytanie wspolrzedych mapy i przekazanie ich do managera
    std::vector<sf::Vector2f> path = PathLoader::loadPath("../Assets/maps/map1.txt");
    manager.setPath(path);




    // SYSTEM WIE¯ 
    
        FZwrotnaObrazen callbackObrazen = PrzyznajObrazenia;
        FUtworzPocisk callbackStworzPocisk = Stworzpocisk;

        //przekazanie callbacka do kierownika wiezy
        KierownikWiezy kierownik_Wiezy(callbackObrazen, callbackStworzPocisk);

        g_kierownikWiezy = &kierownik_Wiezy; // Przypisanie do wskaŸnika globalnego dla callbacku


        // SYSTEM GRY 

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

        //PAUSE MENU 
        sf::Texture pauseTexture;
        sf::Sprite pauseSprite;
        pauseTexture.loadFromFile("../Assets/pause/pause.png");
        pauseSprite.setTexture(pauseTexture);
        sf::Vector2u winSize = window.getSize();
        sf::Vector2u texSize = pauseTexture.getSize();

        pauseSprite.setScale(
            (float)winSize.x / texSize.x,
            (float)winSize.y / texSize.y
        );

        pauseSprite.setPosition(0.f, 0.f);

        sf::Texture resumeTexture;
        sf::Sprite resumeSprite;
        resumeTexture.loadFromFile("../Assets/pause/resume.png");
        resumeSprite.setTexture(resumeTexture);
        resumeSprite.setPosition(0.f, 100.f);

        sf::Texture restartTexture;
        sf::Sprite restartSprite;
        restartTexture.loadFromFile("../Assets/pause/restart.png");
        restartSprite.setTexture(restartTexture);
        restartSprite.setPosition(0.f, 50.f);


        sf::FloatRect przyciskStart(
            524.f,
            642.f,
            997.f - 524.f,
            736.f - 642.f
        );
        sf::FloatRect przyciskReplay(
            520.f,
            570.f,
            1010.f - 520.f,
            675.f - 570.f
        );
        sf::FloatRect przyciskResume(
            524.f,
            490.f,
            997.f - 524.f,
            736.f - 642.f
        );

        // Zmienne steruj¹ce stanem gry
        sf::Clock zegar;
        int currentWave = 0;
        bool graStart = false; //zmienna sprawdzajaca czy gra zostala ropoczeta
        bool gamePaused = false;


        // G³ówna pêtla gry
        while (window.isOpen()) {
            sf::Event event;
            //float dt = zegar.restart().asSeconds();

            //trzeba pobrac czas ktory uplynal od ostatniego momnetu/klatki (dla wie¿)
            float czasDelta = zegar.restart().asSeconds();

            // Obs³uga zdarzeñ systemowych (zamkniêcie okna, sterowanie)
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    g_kierownikWiezy = nullptr;
                    g_enemyManager = nullptr;
                    window.close();
                    break;
                   
                }
                // Logika w trakcie gry
                if (manager.gameOver()) {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
                        // sprawdzenie czy klikniêto na przycisk replay
                        if (przyciskReplay.contains(mousePos)) {
                            // resetowanie stanu gry
                            graStart = false;
                            gamePaused = false;
                            currentWave = 0;
                            manager.reset();
                            kierownik_Wiezy.reset();
                        }
                    }
                }


                if (graStart && !gamePaused && !manager.gameOver()) { //ustawiamy wieze tylko wtedy kiedy gra jest uruchomiona i nie ma game over
                    handleForgeEvent(event, manager); //obsluga kuzni
                }
                //escape
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape)
                {
                    if (graStart && !manager.gameOver()) {
                        gamePaused = !gamePaused;
                    }
                    continue;
                }

				// Obs³uga klikniêæ myszy podczas gry 10.01.2026
                if (graStart && !gamePaused && !manager.gameOver()) {
					if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
						sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
						sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

						//funkcja obsluz klikniecie zebysmy mogli sobie klikaæ na wieze i ulepszac je i zmniejszac jej poziom
						kierownik_Wiezy.ObsluzKlikniecie(worldPos, manager);
                    }

                }

                // Menu przed rozpoczêciem gry
                if (!graStart) {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
                        if (przyciskStart.contains(mousePos)) {
                            graStart = true; //ustawiamy zmienna na true, aby rozpocz gre
                            
                        }
                    }
                    continue;
                }

                if (gamePaused) {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
                        if (przyciskResume.contains(mousePos)) {
                            graStart = true;
							gamePaused = false;
                        }

                        cout << "STRES test dodano 30 celow" << endl;
                    }

                    // if (event.key.code == sf::Keyboard::D) {
                      //   kierownik_Wiezy.UlepszWieze(1); // Ulepsz wieze o ID 1
                     //}
               //  }




                 // do testu, zadanie obrazen przyciskiem myszy
                 // tu bedzie zmiana na wieze
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        // pozycja myszy wzglêdem okna
                        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                        kierownik_Wiezy.ObsluzKlikniecie(worldPos, manager);//sprawdzamy czy kliknieto na wieze i czy ulepszac

                        //    // funkcja zadawania obra¿eñ MYSZY!!!
                        //    manager.MouseClick(worldPos);

                    }
                    else if (przyciskStart.contains(mousePos)) {
                            graStart = false;
                            gamePaused = false;
                            currentWave = 0;
                            manager.reset();
                            kierownik_Wiezy.reset();
                        }
                    }
                    continue;

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


            //aktualizacja gry gdy ta nie jest zatrzymana
            if (graStart && !gamePaused && !manager.gameOver()) {
                manager.update(czasDelta);

                // AKTUALIZACJA WIE¯  
                //zrobienie listy celow na podstawie mapy wrogow
                vector<Cel> aktualna_lista_celow = StworzListeCelow(manager);

                //zaktualizowac system wiez pokazujac mu liste potencjalnych celow
                kierownik_Wiezy.Aktualizuj(czasDelta, aktualna_lista_celow);

            }
       

            // Rysowanie sceny
            window.clear(sf::Color(20, 20, 30)); // Kolor t³a z 

            if (!graStart) {
                window.draw(menuSprite); //rysuje menu
            }
            else if (manager.gameOver()) {
                window.draw(gameOverSprite); // Wyœwietlenie ekranu koñca gry
            }
            else {
                window.setView(worldView);
                // Rysowanie aktywnej rozgrywki
                renderMapa(window); //rysuje mape

                // Rysowanie przeciwników i logiki
                manager.draw(window);

                // Rysowanie  pociskow
                kierownik_Wiezy.RysujDebug(window);

                rysujHUD(window, manager.getPlayerHealth(), manager.getPlayerMoney(), currentWave); //rysuje hud 
                rysujForge(window, manager); //rysuje kuznie 
                if (gamePaused) {
                    window.draw(pauseSprite);
                    window.draw(resumeSprite);
                    window.draw(restartSprite);
                }


            }

            window.display();
        }
    
    g_kierownikWiezy = nullptr;
    g_enemyManager = nullptr;
    return 0;
}

WaveConfig getWaveSettings(int waveNumber) {//scenariusze fali
    WaveConfig config;
    config.delay = std::max(0.3f, 1.0f - (waveNumber * 0.05f));

    int totalEnemies = 1 + (waveNumber*2);//ilu przeciwnikow ma byc w fali

    for (int i = 0; i < totalEnemies; i++) {
        if (waveNumber % 5 == 0) { //boss
            config.kolejnoscEnemies.push_back(3);
            break;
        }
        else if (i % 3 == 0 && waveNumber >= 3) { //co trzeci przeciwnik to typ 1 od rundy 3
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
    return config;
}