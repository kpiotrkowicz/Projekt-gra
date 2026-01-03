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
            para.second.pozycja,
            para.second.zycie});
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

int main() {
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
    

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f myszPozycja(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                //sprawdzamy czy kliknieto na ikone menu
                bool kliknietowWMenu = false;
                for (int i = 0; i < 4; ++i) {
                    if (ikonyMenu[i].getGlobalBounds().contains(myszPozycja)) {
                        wybranyTypWiezy = typyWiez[i];
                        kliknietowWMenu = true;
                        cout << "Wybrano wieze typu: " << wybranyTypWiezy << endl;
                        break;
                    }
                }
                if (!kliknietowWMenu&&myszPozycja.y<500.f) {
                    //jezeli nie kliknieto w menu to stawiamy wieze na mapie
                    kierownik_Wiezy.DodajWieze(myszPozycja, wybranyTypWiezy);
                    cout << "Postawiono wieze typu: " << wybranyTypWiezy << " na pozycji (" << myszPozycja.x << ", " << myszPozycja.y << ")" << endl;
				}
            }
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
		for (const auto& ikona : ikonyMenu) {
            window.draw(ikona);
        }
        window.display();
    }
	return 0;


}
