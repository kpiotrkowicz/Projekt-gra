#pragma once
#include "Interfejs.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
using namespace std;
const int MAKSYMPOZIOM = 3;
// Struktura reprezentujaca potencjalny cel, przekazywania wiezy inf o potencjalnych celach
struct Cel {
	int id; // Unikalne ID celu
	sf::Vector2f pozycja; // Pozycja celu
	float zdrowie; // Zdrowie celu
};

class wieza {
	public:
	// Konstruktor wiezy
	wieza(int id, sf::Vector2f pozycja, float zasieg, float obrazenia, float czasOdnowienia,
		FZwrotnaObrazen callback, FUtworzPocisk callbackPocisk,string typ);
	// Aktualizacja wiezy (wywolywana w kazdej klatce gry)
	void Aktualizuj(float czasDelta, const vector<Cel>& potencjalneCele);
	int PobierzId() const { return id; } // Pobierz unikalne ID wiezy

	float PobierzZasieg() const { return zasieg; } // Pobierz zasieg wiezy
	float PobierzObrazenia() const { return obrazenia; } // Pobierz obrazenia wiezy
	sf::Vector2f PobierzPozycje() const { return pozycja; } // Pobierz pozycje wiezy

	void zasiegDebug(sf::RenderWindow& window); // Funkcja do rysowania zasiegu wiezy (debug)
	// Ustawienie typu targetowania
	bool Ulepsz();
private:
	//lista celow- argument funkcji
	void ZnajdzCel(const vector<Cel>& potencjalneCele);
	void Strzel();
	int poziom;
	int id; // Unikalne ID wiezy
	sf::Vector2f pozycja; // Pozycja wiezy
	sf::Texture tekstura;
	sf::Sprite sprite;
	string typ;

	float zasieg; // Zasieg wiezy
	float obrazenia; // Obrazenia zadawane przez wieze
	float czasOdnowienia; // Czas odnowienia strzalu
	float czasOdOstatniegoStrzalu; // Czas od ostatniego strzalu
	int aktualnyCelId; // ID aktualnego celu (-1 jesli brak celu)

	FZwrotnaObrazen przyznajObrazeniaCallback; // Callback do przyznawania obrazen
	FUtworzPocisk utworzPociskCallback; // Callback do tworzenia pociskow
	string typTargetowania; // Typ targetowania (np. "najblizszy", "najbardziej_zraniony")
};