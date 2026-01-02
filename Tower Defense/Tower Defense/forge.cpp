#include "forge.h"
#include<string>
//tu chodzi o wybor wiez scrollem
int wybranaWieza = 0;
const int liczbaWiez = 4;
bool kuzniaWidoczna = true;

int cenaWiezy[4] = {
	50,   // wieza 1 
	100,  // wieza 2
	200,  // wieza 3
	350   // wieza 4 

};


//wieze
sf::Texture towerTex[liczbaWiez];
sf::Sprite towerSprite[liczbaWiez];

sf::Font forgeFont;
sf::Text cenaTekst[liczbaWiez];

void initForge(sf::RenderWindow& window) {
	forgeFont.loadFromFile("../Assets/fonts/font.ttf");//czcionka do cen
	//pozycjowanie kuzni
	float x = (window.getSize().x / 2.f) - 300.f;
	float y = window.getSize().y - 250.f;
	float startX =20.f;
	
	//ikony wiez
	towerTex[0].loadFromFile("../Assets/hud/tower_1.png");
	towerTex[1].loadFromFile("../Assets/hud/tower_2.png");
	towerTex[2].loadFromFile("../Assets/hud/tower_3.png");
	towerTex[3].loadFromFile("../Assets/hud/tower_4.png");
	for (int i = 0; i < liczbaWiez; i++) {
		towerSprite[i].setTexture(towerTex[i]);
		towerSprite[i].setPosition(
			startX + i * 110.f,
			y
		);

		cenaTekst[i].setFont(forgeFont);
		cenaTekst[i].setCharacterSize(20);
		cenaTekst[i].setString(std::to_string(cenaWiezy[i]));
		cenaTekst[i].setFillColor(sf::Color::White);
		cenaTekst[i].setPosition(
			startX + i * 110.f,
			y + 55.f   
		);
	}
}
	


void handleForgeEvent(const sf::Event& event, EnemyManager& manager) {
	//tab- ukrywamy kuznie

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
		kuzniaWidoczna = !kuzniaWidoczna;
	}
	if (!kuzniaWidoczna) return;

	//scroll-wyborwiezy
	if (event.type == sf::Event::MouseWheelScrolled) {
		if (event.mouseWheelScroll.delta > 0) {
			wybranaWieza--;
			if (wybranaWieza < 0) wybranaWieza = liczbaWiez - 1;
		}
		else if (event.mouseWheelScroll.delta < 0) {
			wybranaWieza++;
			if (wybranaWieza >= liczbaWiez) wybranaWieza = 0;
		}
	}
		//kupno wiezy
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
			if (manager.getPlayerMoney() >= cenaWiezy[wybranaWieza]) {
				manager.moneySum( -cenaWiezy[wybranaWieza]);
				//tu potem ma byc stawianie wiezy!!!!
			}
	}
	}

	void rysujForge(sf::RenderWindow& window){
		if (!kuzniaWidoczna) return;

		int aktualneZloto = manager.getPlayerMoney();

		for (int i = 0; i < liczbaWiez; i++) {

			if (i == wybranaWieza) {
				towerSprite[i].setColor(sf::Color::White);
				towerSprite[i].setScale(0.1f, 0.1f);
			}
			else {
				towerSprite[i].setColor(sf::Color(150, 150, 150));
				towerSprite[i].setScale(0.1f, 0.1f);
			}
			if (aktualneZloto < cenaWiezy[i]) //ceny wiez
				cenaTekst[i].setFillColor(sf::Color(180, 50, 50));
			else
				cenaTekst[i].setFillColor(sf::Color::White);

			window.draw(towerSprite[i]);
			window.draw(cenaTekst[i]);

		}
	
	}

