#pragma once

#include "wieza.h"
#include "pocisk.h"
#include <memory>
#include <vector>

using namespace std;

class KierownikWiezy {
public:

	KierownikWiezy();
	void Aktualizuj(float czasDelta, const vector<Cel>& cele);
	void zasiegDebug(sf::RenderWindow& window); // Funkcja do rysowania zasiegu wiez (debug)
	//f do stawiania/budoawania wiez
	void DodajWieze(sf::Vector2f pozycja, float zasieg, float obrazenia, float czasOdnowienia);

private:
	vector<wieza> wieze; //lista wiez
	//unikalne id dla wiez zeby moc zarzadac pociskami
	vector<unique_ptr<pocisk>> pociski; //lista pociskow zarzadzana przez kierownika wiezy

	int nastepneIdWiezy = 1; //unikalne id dla wiez
	int nastepneIdPocisku = 1; //unikalne id dla pociskow

	//callbacki- funckje zwrotne do przekazania do wiez i pociskow
	void PrzyznajObrazenia(int celId, float ilosc);
	void UtworzPocisk(int wiezaId, int celId, sf::Vector2f pozycjaStartowa, float obrazenia);
};

