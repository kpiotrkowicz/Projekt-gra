#include <SFML/Graphics.hpp>
#include "KierownikWiezy.h"
#include <functional>//dla funtion
#include "zabojcacelow.h"
#include "interfejs.h"
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;

//funkcje do sceny testowej 
map<int, ZabojcaCelow> mapa_wrogow;
#include <iostream>
#include <vector>
#include "EnemyManager.h"
#include "PathLoader.h"
#include "mapa.h"
#include "hud.h"
#include "forge.h"

//funkcja ktora wywola kierownik wiezy gdy pocisk trafi w cel
static void PrzyznajObrazenia(int celId, float ilosobrazen) {
    auto it = mapa_wrogow.find(celId);
    if (it != mapa_wrogow.end()) {
        it->second.obrazeniacelu(ilosobrazen);
      //usuwamy w usunpokonanecele
    }
    else {
        cout << "Cel o ID: " << celId << " nie istnieje!" << endl;
    }
};

//tworzenie celow - funkcja wywolywana przez wieze przy strzale
static vector<Cel>StworzListeCelow() {
vector<Cel>lista_celow;
for (const auto& para : mapa_wrogow) {//iteracja po mapie
    if (para.second.zycie > 0.0f) {
        lista_celow.push_back({ 
            
            para.second.id, 
            para.second.pozycja });
    }
}return lista_celow;
};

static void usunpokonanecele() {
    for (auto it = mapa_wrogow.begin(); it != mapa_wrogow.end();) {
        if (it->second.zycie <= 0.0f) {
            it = mapa_wrogow.erase(it); // Usun cel z mapy, jesli zostal zniszczony
        }
        else {
            ++it;
        }

    }
};

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
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML dzia³a!");
	sf::Clock zegar;
	
	FZwrotnaObrazen callbackObrazen = PrzyznajObrazenia;
	//przekazanie callbacka do kierownika wiezy
	KierownikWiezy kierownik_Wiezy(callbackObrazen);

	mapa_wrogow.emplace(10, ZabojcaCelow(10, { 150.f,150.f }, 100.f));
	mapa_wrogow.emplace(11, ZabojcaCelow(11, { 400.f,300.f }, 150.f));
    mapa_wrogow.emplace(12, ZabojcaCelow(12, { 600.f,500.f }, 50.f));


    //stawiam pare wiez testowych(pozycja,typ)
    kierownik_Wiezy.DodajWieze({ 100.f,100.f }, "tower_1");
    kierownik_Wiezy.DodajWieze({ 300.f,400.f }, "tower_2");


    //testowa lista ceklow
    vector<Cel> cele;
	cele.push_back({ 10, {150.f,150.f} });
	cele.push_back({ 11, {400.f,300.f} });


    // G³ówna pêtla gry
    while (window.isOpen()) {
        sf::Event event;
        float dt = clock.restart().asSeconds();

        // Obs³uga zdarzeñ systemowych (zamkniêcie okna, sterowanie)
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::S) {
                    for (int i = 0; i < 30; i++) {
                        int id = 100 + i;
                        mapa_wrogow.emplace(id, ZabojcaCelow(id, { (float)(rand() % 600 + 100),(float)(rand() % 400 + 100) }, 50.f));
                    }
                    cout << "STRES test dodano 30 celow" << endl;
                }

                if (event.key.code == sf::Keyboard::D) {
                    kierownik_Wiezy.UlepszWieze(1); // Ulepsz wieze o ID 1
                }
            }
        }

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
            rysujHUD(window, manager.getPlayerHealth(), manager.getPlayerMoney(), currentWave); //rysuje hud 
            rysujForge(window, manager); //rysuje kuznie 
        }

    
     
   //trzeba pobrac czas ktory uplynal od ostatniego momnetu/klatki
		float czasDelta = zegar.restart().asSeconds();

		//zrobienie listy celow na podstawie mapy wrogow
		vector<Cel> aktualna_lista_celow = StworzListeCelow();


        //zaktualizowac system wiez pokazujac mu liste potencjalnych celow
		kierownik_Wiezy.Aktualizuj(czasDelta, aktualna_lista_celow);

		//usuwamy pokonane cele z mapy
		usunpokonanecele();

        window.clear(sf::Color(20,20,30));
            
		

		// Rysowanie  pociskow (debug)
		for(const auto& para : mapa_wrogow) {
            sf::RectangleShape ksztaltCelu({ 20.f, 20.f });
            ksztaltCelu.setOrigin(10.f, 10.f);
            ksztaltCelu.setPosition(para.second.pozycja);
            ksztaltCelu.setFillColor(sf::Color::Green);
            window.draw(ksztaltCelu);
        }
		
		kierownik_Wiezy.RysujDebug(window);
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