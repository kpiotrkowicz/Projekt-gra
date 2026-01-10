#include "forge.h"
#include "KierownikWiezy.h"
#include <vector>
#include <string>
#include <cmath>

extern KierownikWiezy* g_kierownikWiezy;
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
	
float dystansPunktuOdOdcinka(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b) {
	sf::Vector2f ab = b - a;
	sf::Vector2f ap = p - a;
	float dlugoscKwadrat = ab.x * ab.x + ab.y * ab.y;
	if (dlugoscKwadrat == 0.0f) return std::sqrt(ap.x * ap.x + ap.y * ap.y);
	float t = std::max(0.0f, std::min(1.0f, (ap.x * ab.x + ap.y * ab.y) / dlugoscKwadrat));
	sf::Vector2f najblizszy = a + ab * t;
	sf::Vector2f diff = p - najblizszy;
	return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}


void handleForgeEvent(const sf::Event& event, EnemyManager& manager) {
	if (g_kierownikWiezy == nullptr) return;
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

				sf::Vector2f mousePos(
					static_cast<float>(event.mouseButton.x),
					static_cast<float>(event.mouseButton.y)
				);
				if (g_kierownikWiezy != nullptr) {
					if (mousePos.y < (1024 - 50.f) && mousePos.y >(150.f)) {//abye nie mozna bylo postaiwc za wysoko lub za nisko 
						float marginesDrogi = 60.0f;
						float marginesWiezy = 60.0f;

						bool kolizjaDroga = false;
						const auto& sciezka = manager.getPath(); //funkcja z EnemyManager.h
						for (size_t i = 0; i < sciezka.size() - 1; ++i) {
							if (dystansPunktuOdOdcinka(mousePos, sciezka[i], sciezka[i + 1]) < marginesDrogi) {
								kolizjaDroga = true;
								break;
							}
						}
						bool kolizjaInnaWieza = false;
						if (g_kierownikWiezy && g_kierownikWiezy->kolizjaWiezy(mousePos, marginesWiezy)) {
							kolizjaInnaWieza = true;
						}


						if (!kolizjaDroga && !kolizjaInnaWieza) {
							manager.moneySum(-cenaWiezy[wybranaWieza]); //odejmij pieniadze
							std::string typy[] = { "tower_1","tower_2","tower_3","tower_4" };
							if (g_kierownikWiezy)

							{
								g_kierownikWiezy->DodajWieze(mousePos, typy[wybranaWieza]);
							};
						}
						else {
							std::cout << "Nie mozna budowac na drodze!" << std::endl;
						}
					}
				}
			}
		}
	}

	void rysujForge(sf::RenderWindow& window, EnemyManager& manager){
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

