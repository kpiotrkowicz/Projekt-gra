#pragma once

#include "wieza.h"
#include "pocisk.h"
#include <memory>
#include <vector>
#include <list>//dla listy pociskow

#include <string>

using namespace std;

class KierownikWiezy {
public:

	KierownikWiezy(FZwrotnaObrazen zewnetrznyCallbackObrazen);
	KierownikWiezy() = delete;//usuenelam domyslny konstruktor zeby nie nylo bledow przy tworzeniu kierownika wiezy bez callbacku
	void Aktualizuj(float czasDelta, const vector<Cel>& cele);
	void zasiegDebug(sf::RenderWindow& window); // Funkcja do rysowania zasiegu wiez (debug)
	//f do stawiania/budoawania wiez
	void DodajWieze(sf::Vector2f pozycja, string typWiezy);
	//bool UlepszWieze(int id_wiezy);

private:
	vector<wieza> wieze; //lista wiez
	//unikalne id dla wiez zeby moc zarzadac pociskami
	vector<unique_ptr<pocisk>> pociski; //lista pociskow zarzadzana przez kierownika wiezy

	int nastepneIdWiezy = 1; //unikalne id dla wiez
	int nastepneIdPocisku = 1; //unikalne id dla pociskow
	FZwrotnaObrazen zewnetrznyCallbackObrazen;

	//callbacki- funckje zwrotne do przekazania do wiez i pociskow
	void PrzyznajObrazenia(int celId, float ilosc);
	void UtworzPocisk(int wiezaId, int celId, sf::Vector2f pozycjaStartowa, float obrazenia);
};

