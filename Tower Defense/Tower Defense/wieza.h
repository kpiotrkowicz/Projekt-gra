#pragma once
#include "Interfejs.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
using namespace std;

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
	bool Ulepsz(int& portfel);
	bool ZmniejszPoziom(int& portfel);
	bool doUsuniecia = false;
	int dajPoziom() const { 
		return poziom; 
	}
	bool czyKliknieto(sf::Vector2f mousePos) const {
		
		return sprite.getGlobalBounds().contains(mousePos);
	}
	float pobierzCzasIstnienia() const { return zegarIstnienia.getElapsedTime().asSeconds(); }
	void resetujUsuniecie() { czekajNaUsuniecie = false; } // Pozwala "odwo³aæ" próbê sprzeda¿y
private:
	//lista celow- argument funkcji
	sf::Clock zegarIstnienia;
	bool czekajNaUsuniecie = false;//flaga czy wieza ma byc usunieta jak zmniejszamy poziom
	int kosztUlepszenia;
	int cenaSprzedazy ;
	void ZnajdzCel(const vector<Cel>& potencjalneCele);
	void Strzel();
	int poziom=1;
	static const int MAKSYMPOZIOM = 5;
	int id; // Unikalne ID wiezy
	sf::Vector2f pozycja; // Pozycja wiezy
	sf::Texture tekstura;
	sf::Sprite sprite;
	string typ;

	sf::Font czcionka;
	sf::Text informacyjny;
	float czasWyswietlaniatekstu = 0.0f;

	float zasieg; // Zasieg wiezy
	float obrazenia; // Obrazenia zadawane przez wieze
	float czasOdnowienia; // Czas odnowienia strzalu
	float czasOdOstatniegoStrzalu; // Czas od ostatniego strzalu
	int aktualnyCelId; // ID aktualnego celu (-1 jesli brak celu)

	FZwrotnaObrazen przyznajObrazeniaCallback; // Callback do przyznawania obrazen
	FUtworzPocisk utworzPociskCallback; // Callback do tworzenia pociskow
	string typTargetowania; // Typ targetowania (np. "najblizszy", "najbardziej_zraniony")
	//sf::Text tekstPoziom;
	sf::Texture teksturyPoziomow[MAKSYMPOZIOM]; // Tekstury dla kazdego poziomu wiezy
	sf::Sprite spritePoziom; // Sprite do wyswietlania poziomu wiezy
	
};