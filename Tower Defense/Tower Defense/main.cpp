#include <SFML/Graphics.hpp>
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


//funkcje do sceny testowej 
// niepotrzbne?
//map<int, ZabojcaCelow> mapa_wrogow;


//wskaznik globalny do kierownika wrogow aby bylo mozna z niego korzystac w callbacku
EnemyManager* g_enemyManager = nullptr;

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
        lista_celow.push_back({ wrog.getID(), wrog.getPosition() });
    }

    //for (const auto& para : mapa_wrogow) {//iteracja po mapie
    //    if (para.second.zycie > 0.0f) {
    //        lista_celow.push_back({

    //            para.second.id,
    //            para.second.pozycja });
    //    }
    //}
    
    
    return lista_celow;
};


//funkcja niepotrzebna?
// jezeli ptowor nie ma zycia to usuwamy go z mapy 
//static void usunpokonanecele() {
//    for (auto it = mapa_wrogow.begin(); it != mapa_wrogow.end();) {
//        if (it->second.zycie <= 0.0f) {
//            it = mapa_wrogow.erase(it); // Usun cel z mapy, jesli zostal zniszczony
//        }
//        else {
//            ++it;
//        }
//
//    }
//};



WaveConfig getWaveSettings(int waveNumber); //funkcja do ustawiania poziomu fal (napisana na dole)

int main() {
<<<<<<< HEAD
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML dzia³a!");
	sf::Clock zegar;
	
	FZwrotnaObrazen callbackObrazen = PrzyznajObrazenia;
	//przekazanie callbacka do kierownika wiezy
	KierownikWiezy kierownik_Wiezy(callbackObrazen);

	mapa_wrogow.emplace(10, ZabojcaCelow(10, { 150.f,150.f }, 100.f));
	mapa_wrogow.emplace(11, ZabojcaCelow(11, { 400.f,300.f }, 150.f));
    mapa_wrogow.emplace(12, ZabojcaCelow(12, { 600.f,500.f }, 50.f));
    mapa_wrogow.emplace(10, ZabojcaCelow(10, { 150.f,190.f }, 100.f));
    mapa_wrogow.emplace(11, ZabojcaCelow(11, { 450.f,300.f }, 150.f));
    mapa_wrogow.emplace(12, ZabojcaCelow(12, { 609.f,570.f }, 50.f));


    //stawiam pare wiez testowych(pozycja,typ)
    kierownik_Wiezy.DodajWieze({ 100.f,150.f }, "tower_1");
    kierownik_Wiezy.DodajWieze({ 250.f,150.f }, "tower_2");
    kierownik_Wiezy.DodajWieze({ 400.f,150.f }, "tower_3");
    kierownik_Wiezy.DodajWieze({ 550.f,150.f }, "tower_4");


   
	
    vector<sf::Sprite> ikonyMenu(4);
	vector<sf::Texture> teksturyMenu(4);
    vector<string>typyWiez = {"tower_1","tower_2", "tower_3","tower_4"};
    for (int i = 0; i < 4; ++i) {
		if (teksturyMenu[i].loadFromFile(typyWiez[i] + ".png")) {
            ikonyMenu[i].setTexture(teksturyMenu[i]);
			//trzeba zesksalpwac te obfazki do rozmiarow ikonek menu
			float skalaX = 50.f / ikonyMenu[i].getLocalBounds().width;
			ikonyMenu[i].setScale(skalaX, skalaX);
            ikonyMenu[i].setPosition(50.f + i * 100.f, 520.f);
            
        }
        else {
            cout << "Nie mozna zaladowac tekstury: " << typyWiez[i] << endl;
        }
    }

	string wybranyTypWiezy = "tower_1";//domyslnie wybieramy narazie wieze 1
    
=======
    // Konfiguracja okna wyœwietlania i limitu klatek na sekundê 
    sf::RenderWindow window(sf::VideoMode(1536, 1024), "Tower Defense !");
    window.setFramerateLimit(60);
>>>>>>> fa1b188b3471b8222dbfd54a6350e735fd593471

    // Inicjalizacja managera przeciwników(bardzo wazne klasa dla gracza i przeciwknika, duzo sie na niej opiera logiki
    EnemyManager manager;
    g_enemyManager = &manager; // Przypisanie do wskaŸnika globalnego dla callbacku

    // Wczytanie wspolrzedych mapy i przekazanie ich do managera
    std::vector<sf::Vector2f> path = PathLoader::loadPath("../Assets/maps/map1.txt");
    manager.setPath(path);




    // SYSTEM WIE¯ 

    FZwrotnaObrazen callbackObrazen = PrzyznajObrazenia;
    //przekazanie callbacka do kierownika wiezy
    KierownikWiezy kierownik_Wiezy(callbackObrazen);

    /*mapa_wrogow.emplace(10, ZabojcaCelow(10, { 150.f,150.f }, 100.f));
    mapa_wrogow.emplace(11, ZabojcaCelow(11, { 400.f,300.f }, 150.f));
    mapa_wrogow.emplace(12, ZabojcaCelow(12, { 600.f,500.f }, 50.f));*/

    //stawiam pare wiez testowych(pozycja,typ)
    /*kierownik_Wiezy.DodajWieze({ 100.f,100.f }, "tower_1");
    kierownik_Wiezy.DodajWieze({ 300.f,400.f }, "tower_2");*/


    
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

    sf::FloatRect przyciskStart(450.f, 300.f, 300.f, 80.f);

    // Zmienne steruj¹ce stanem gry
    sf::Clock zegar;
    int currentWave = 0;
    bool graStart = false; //zmienna sprawdzajaca czy gra zostala ropoczeta
    bool gamePaused = false;



    kierownik_Wiezy.DodajWieze({ 100.f,100.f }, "tower_1");
    kierownik_Wiezy.DodajWieze({ 300.f,400.f }, "tower_2");


    // G³ówna pêtla gry
    while (window.isOpen()) {
        sf::Event event;
        //float dt = zegar.restart().asSeconds();
       
        //trzeba pobrac czas ktory uplynal od ostatniego momnetu/klatki (dla wie¿)
        float czasDelta = zegar.restart().asSeconds();

        // Obs³uga zdarzeñ systemowych (zamkniêcie okna, sterowanie)
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            handleForgeEvent(event, manager); //obsluga kuzni

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

                


                if (event.type == sf::Event::KeyPressed) {
                    /*if (event.key.code == sf::Keyboard::S) {
                        for (int i = 0; i < 30; i++) {
                            int id = 100 + i;
                            mapa_wrogow.emplace(id, ZabojcaCelow(id, { (float)(rand() % 600 + 100),(float)(rand() % 400 + 100) }, 50.f));
                        }
                        cout << "STRES test dodano 30 celow" << endl;
                    }*/

                    if (event.key.code == sf::Keyboard::D) {
                        kierownik_Wiezy.UlepszWieze(1); // Ulepsz wieze o ID 1
                    }
                }




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
            manager.update(czasDelta);

            // AKTUALIZACJA WIE¯  
            //zrobienie listy celow na podstawie mapy wrogow
            vector<Cel> aktualna_lista_celow = StworzListeCelow(manager);

            //zaktualizowac system wiez pokazujac mu liste potencjalnych celow
            kierownik_Wiezy.Aktualizuj(czasDelta, aktualna_lista_celow);

            //usuwamy pokonane cele z mapy
            //usunpokonanecele();
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
            // Rysowanie aktywnej rozgrywki
            renderMapa(window); //rysuje mape

            // Rysowanie przeciwników i logiki
            manager.draw(window);

            // --- RYSOWANIE WIE¯ I CELÓW TESTOWYCH (Z Kodu 1) ---
            // Rysowanie  pociskow (debug)
            /*for (const auto& para : mapa_wrogow) {
                sf::RectangleShape ksztaltCelu({ 20.f, 20.f });
                ksztaltCelu.setOrigin(10.f, 10.f);
                ksztaltCelu.setPosition(para.second.pozycja);
                ksztaltCelu.setFillColor(sf::Color::Green);
                window.draw(ksztaltCelu);
            }*/

            // Rysowanie  pociskow
            kierownik_Wiezy.RysujDebug(window);

            rysujHUD(window, manager.getPlayerHealth(), manager.getPlayerMoney(), currentWave); //rysuje hud 
            rysujForge(window, manager); //rysuje kuznie 
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