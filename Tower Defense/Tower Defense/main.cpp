#include <SFML/Graphics.hpp>

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
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
