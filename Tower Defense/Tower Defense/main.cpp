#include <SFML/Graphics.hpp>
#include "KierownikWiezy.h"
#include <vector>
using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML dzia³a!");
	sf::Clock zegar;
	KierownikWiezy kierownikWiezy;

    //stawiam pare wiez testowych
    kierownikWiezy.DodajWieze({ 100.f,100.f }, 200.f, 10.f, 2.0f);
    kierownikWiezy.DodajWieze({ 300.f,400.f }, 150.f, 5.f, 1.0f);

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
        //zaktualizowac system wiez pokazujac mu liste potencjalnych celow
		kierownikWiezy.Aktualizuj(czasDelta, cele);

        window.clear(sf::Color::Black);


		// Rysowanie  pociskow (debug)
		foe(const auto& cel : cele) {
            sf::RectangleShape ksztatlCelu(20.f,20.f);
            ksztaltCelu.setOrigin(5.f, 5.f);
            ksztaltCelu.setPosition(cel.pozycja);
            ksztaltCelu.setFillColor(sf::Color::Green);
            window.draw(ksztaltCelu);
        }
		
		kierownikWiezy.zasiegDebug(window);
        window.display();
    }
	return 0;


}
