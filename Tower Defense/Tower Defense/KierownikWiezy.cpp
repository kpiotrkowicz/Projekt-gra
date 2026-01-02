#include "KierownikWiezy.h"
#include <iostream>
#include <functional>
#include <list>
#include <string>
#include <algorithm>//zebysmy mogli usuwac pociski z wektora
using namespace std;
using namespace std::placeholders;

KierownikWiezy::KierownikWiezy(FZwrotnaObrazen zewnetrznyCallbackObrazen)
	: zewnetrznyCallbackObrazen(zewnetrznyCallbackObrazen) {
	// Konstruktor kierownika wiezy
	cout << "Kierownik utworzony.\n" << endl;
}


void KierownikWiezy::Aktualizuj(float czasDelta, const vector<Cel>& cele) {
	// Aktualizuj wszystkie wieze- szukaja cele i w nie strzelaja 

	for (auto& wieza_obiekt : wieze) {
		wieza_obiekt.Aktualizuj(czasDelta, cele);
	}

	// Aktualizacja wszystkichh pociskow
	for (auto it = pociski.begin(); it != pociski.end();) {
		int id_celu = (*it)->PobierzIdCelu();
		auto cel_it = find_if(cele.begin(), cele.end(),
			[id_celu](const Cel& cel) 
			{ return cel.id == id_celu; });
		// Znajdz pozycje celu pocisku

		sf::Vector2f pozycjaCelu;
		if (cel_it != cele.end()) {
			pozycjaCelu = cel_it->pozycja;
		}
		else {
			// Cel nie istnieje, usun pocisk
			it = pociski.erase(it);
			continue;
		}

		(*it)->Aktualizuj(czasDelta, pozycjaCelu);
		if (!(*it)->CzyZywy()) {
		it=pociski.erase(it); // Usun pocisk jesli nie zywy
		}
		else {
			++it;
		}
	}
}


	//zwalniamiy pamiec i usuwamy niepotrzebne pociski
	/*pociski.erase(remove_if(pociski.begin(), pociski.end(),
		[](const unique_ptr<pocisk>& p) { return !p->CzyZywy(); }),
		pociski.end());//tymczasowa zmienna p to kazdy pocisk z wektora pociski jesli p nie zywy to usuwamy go z wektora

}*/


	// Funkcja callback do przyznawania obrazen gdy pocisk trafia w cel
	
	void KierownikWiezy::PrzyznajObrazenia(int celId, float ilosc) {
		if (zewnetrznyCallbackObrazen) {
			zewnetrznyCallbackObrazen(celId, ilosc);
		}
	}


void KierownikWiezy::RysujDebug(sf::RenderWindow& window) {
	// Rysuj zasieg wszystkich wiez 
	for (auto& wieza_obiekt : wieze) {
		wieza_obiekt.zasiegDebug(window);
	}
	// Rysuj pociski 
	for (auto& pocisk_obiekt : pociski) {
		pocisk_obiekt->zasiegDebug(window);
	}
}

/*void KierownikWiezy::DodajWieze(sf::Vector2f pozycja, string typ_wiezy) {
	// Tworzenie callbackow
	FZwrotnaObrazen callbackObrazen = bind(&KierownikWiezy::PrzyznajObrazenia, this, placeholders::_1, placeholders::_2);
	FUtworzPocisk callbackPocisk = bind(&KierownikWiezy::UtworzPocisk, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	float zasieg=150.0f;
	float obrazenia=10.0f;
	float czasOdnowienia=1.5f;

	// Dodaj nowa wieze do listy
	wieze.emplace_back(nastepneIdWiezy++,
		pozycja, zasieg,
		obrazenia, czasOdnowienia,
		callbackObrazen, callbackPocisk);
}
*/
void KierownikWiezy::DodajWieze(sf::Vector2f pozycja, string typ) {
	float zasieg=150.0f;
	float obrazenia = 10.0f;
	float czasOdnowienia = 1.5f;

	if (typ == "tower_1") {//wieza lucznikow ta slomiana 
		zasieg = 220.0f;
		obrazenia = 12.0f;
		czasOdnowienia = 0.6f;
	}
	if (typ == "tower_2") {//wieza ognia ta kamienna z plomieniem 
		zasieg = 160.0f;
		obrazenia = 35.0f;
		czasOdnowienia = 1.8f;
	}
	if (typ == "tower_3") {//wieza lodowa- krysztalowa
		zasieg = 140.0f;
		obrazenia = 20.0f;
		czasOdnowienia = 1.2f;
	}
	if (typ == "tower_4") {//wieza magzczna zlota
		zasieg = 190.0f;
		obrazenia = 50.0f;
		czasOdnowienia = 2.5f;
	}
	wieze.emplace_back(nastepneIdWiezy++,
		pozycja, zasieg,
		obrazenia, czasOdnowienia, zewnetrznyCallbackObrazen, bind(&KierownikWiezy::UtworzPocisk, this, _1,_2,_3,_4),typ);
}


void KierownikWiezy::UtworzPocisk(int wiezaId, int celId, sf::Vector2f pozycjaStartowa, float obrazenia) {
	// Tworzenie callbacku do przyznawania obrazen
	FZwrotnaObrazen callbackObrazen = bind(&KierownikWiezy::PrzyznajObrazenia,this,placeholders::_1, placeholders::_2);
		
	
	// Dodaj nowy pocisk do listy
	pociski.emplace_back(make_unique<pocisk>(
		nastepneIdPocisku++,
		celId,
		pozycjaStartowa,
		obrazenia,
		callbackObrazen));
}

bool KierownikWiezy::UlepszWieze(int id_wiezy) {
	// Znajdz wieze o danym ID i ulepsz jej parametry
	for (auto& wieza_obiekt : wieze) {
		if (wieza_obiekt.PobierzId() == id_wiezy) {
			// Przykadowe ulepszenie: zwiekszenie zasiegu i obrazen
			return wieza_obiekt.Ulepsz();
		}
	}
	cout << "Wieza o ID: " << id_wiezy << " nie znaleziona.\n" << endl;
	return false; // Nie znaleziono wiezy o podanym ID
}