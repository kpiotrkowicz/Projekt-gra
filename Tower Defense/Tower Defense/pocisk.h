#pragma once
#include "Interfejs.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class pocisk {//pojedynczy pocisk wystrzelony przez wieze, istnieje od momentu strzalu do trafienia lub znikniecia celu poza zasiegiem 
	// sf::Vector2f pozycja; //pozycja pocisku wspolrzedne x y na mapie 
	//bool czy_zywy; //czy pocisk jest aktywny (czy ma leciec do celu) informuje kierownika wiezy czyli taka flaga zycia pocisku

public:
	pocisk(int id, int id_celu, sf::Vector2f pozycjaStartowa, float obrazenia, FZwrotnaObrazen callback);

	void Aktualizuj(float czasdelta, sf::Vector2f pozycjaCelu);
	void zasiegDebug(sf::RenderWindow& window) const;
	bool CzyZywy() const { return czy_zywy; }
	int PobierzIdCelu() const { return id_celu; }
	bool czy_zywy = true; //czy pocisk jest aktywny i czy jest zuzyty czy nie 
	float PobierzObrazenia() const { return obrazenia; }
	void UstawNowyCel(int noweIdCelu) { id_celu = noweIdCelu; }//ustawienie nowego celu dla pocisku
	sf::Vector2f Pobierzpozycje() const { return pozycja; }//pobieramy pozycje pocisku
private:
	int id; //unikalne id pocisku
	int id_celu; //id celu do ktorego leci pocisk
	sf::Vector2f pozycja; //pozycja pocisku
	float predkosc=400.0f; //predkosc pocisku (stala)
	float obrazenia; //obrazenia jakie zadaje pocisk
	FZwrotnaObrazen przyznajObrazeniaCallback; //callback do przyznawania obrazen


};