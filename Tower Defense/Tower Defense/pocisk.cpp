#include "Pocisk.h"
#include <cmath>

using namespace std;

pocisk::pocisk(int id, int id_celu, sf::Vector2f pozycjaStartowa, float obrazenia, FZwrotnaObrazen callback)
	: id(id), id_celu(id_celu), pozycja(pozycjaStartowa), czy_zywy(true),obrazenia(obrazenia), przyznajObrazeniaCallback(callback)
{
	cout << "Pocisk o ID: " << id << " leci do celu o ID: " << id_celu << " z pozycji (" << pozycja.x << ", " << pozycja.y << ")\n" << endl;
}

void pocisk::Aktualizuj(float czas_delta, sf::Vector2f pozycjaCelu)
{
	if (!czy_zywy) return;

	// Oblicz wektor kierunku do celu
	sf::Vector2f kierunek = pozycjaCelu - pozycja;
	float odleglosc = sqrt(kierunek.x * kierunek.x + kierunek.y * kierunek.y);
	// Sprawdz czy pocisk dotarl do celu


	if (odleglosc > 0)
	{
		kierunek.x = kierunek.x / odleglosc;
		kierunek.y = kierunek.y / odleglosc;

	}
	//trzeba obliczyc ruch na ten czas/ klatke 

	sf::Vector2f ruch = kierunek * predkosc * czas_delta;
	float dystansRuchu = sqrt(ruch.x * ruch.x + ruch.y * ruch.y);

	//sprawdzic trzeba czy pocisk doleci do celu w tym czasien czy tam naszej klatce 

	if (dystansRuchu >= odleglosc) {
		pozycja = pozycjaCelu;//nasz cel jest trafiony 
		//wiec trzeba zadac obrazenia - callback obrazen
		if (przyznajObrazeniaCallback) {
			przyznajObrazeniaCallback(id_celu, obrazenia);
		}
		czy_zywy = false; // Pocisk znikna po trafieniu- usuwamy go

	}
	else
	{
		pozycja += ruch; // Aktualizuj pozycje pocisku i kontynuuje lot 
	}
}
	void pocisk::zasiegDebug(sf::RenderWindow & window) const {
		if (czy_zywy==false) return;
		sf::CircleShape kropka(5.f);
		kropka.setOrigin(5.f, 5.f);
		kropka.setPosition(pozycja);
		kropka.setFillColor(sf::Color::Red);
		window.draw(kropka);
	}

