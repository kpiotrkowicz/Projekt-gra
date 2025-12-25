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

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML dzia³a!");
	sf::Clock zegar;
	
	FZwrotnaObrazen callbackObrazen = PrzyznajObrazenia;
	//przekazanie callbacka do kierownika wiezy
	KierownikWiezy kierownik_Wiezy(callbackObrazen);

	mapa_wrogow.emplace(10, ZabojcaCelow(10, { 150.f,150.f }, 100.f));
	mapa_wrogow.emplace(11, ZabojcaCelow(11, { 400.f,300.f }, 150.f));
    mapa_wrogow.emplace(12, ZabojcaCelow(12, { 600.f,500.f }, 50.f));


    //stawiam pare wiez testowych(pozycja,typ)
    kierownik_Wiezy.DodajWieze({ 100.f,100.f }, "Lucznik");
    kierownik_Wiezy.DodajWieze({ 300.f,400.f }, "Armata");


    //testowa lista ceklow
    vector<Cel> cele;
	cele.push_back({ 10, {150.f,150.f} });
	cele.push_back({ 11, {400.f,300.f} });


    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
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
            
		//kierownik_Wiezy.DodajWieze(window);// rysowanie wiez

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
