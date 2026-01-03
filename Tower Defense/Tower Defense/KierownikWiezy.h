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

	KierownikWiezy(FZwrotnaObrazen zewnetrznyCallbackObrazen, FUtworzPocisk callbackStworzPocisk);
	KierownikWiezy() = delete;//usuenelam domyslny konstruktor zeby nie nylo bledow przy tworzeniu kierownika wiezy bez callbacku
	void Aktualizuj(float czasDelta, const vector<Cel>& cele);
	void RysujDebug(sf::RenderWindow& window); // Funkcja do rysowania wiez (debug)
	//f do stawiania/budoawania wiez
	void DodajWieze(sf::Vector2f pozycja, string typWiezy);
	int PobierzId() const { return id; }//pobiera unikalne id kierownika wiezy
	sf::Vector2f PobierzPozycjeW() const { return pozycja; }
	void PrzyznajObrazenia(int celId, float ilosc);
	bool UlepszWieze(int id_wiezy);
	void UtworzPocisk(int wiezaId, int celId, sf::Vector2f pozycjaStartowa, float obrazenia);
private:
	vector<wieza> wieze; //lista wiez
	//unikalne id dla wiez zeby moc zarzadac pociskami
	vector<unique_ptr<pocisk>> pociski; //lista pociskow zarzadzana przez kierownika wiezy

	int nastepneIdWiezy = 1; //unikalne id dla wiez
	int nastepneIdPocisku = 1; //unikalne id dla pociskow
	FZwrotnaObrazen zewnetrznyCallbackObrazen;
	int id; //unikalne id kierownika wiezy
	sf::Vector2f pozycja; //pozycja kierownika wiezy na mapie
	//callbacki- funckje zwrotne do przekazania do wiez i pociskow
	
	
};

