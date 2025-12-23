#include "KierownikWiezy.h"
#include <iostream>
#include <functional>
#include <algorithm>//zebysmy mogli usuwac pociski z wektora
using namespace std;

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
	for (auto& pocisk_obiekt : pociski) {
		// Znajdz pozycje celu pocisku
		if (pocisk_obiekt->CzyZywy()) {
			sf::Vector2f pozycjaCelu;
			bool cel_znaleziony = false;

			//szuka pozycji celu w liscie celow
			for (const auto& cel : cele) {
				if (cel.id == pocisk_obiekt->PobierzIdCelu()) {
					pozycjaCelu = cel.pozycja;
					cel_znaleziony = true;
					break;
				}
			}

			if (cel_znaleziony) {
				pocisk_obiekt->Aktualizuj(czasDelta, pozycjaCelu);
			}
			else {
				// Cel nie istnieje, pocisk powinien zniknac
				pocisk_obiekt->czy_zywy = false;
			}
		}
	}

	//zwalniamiy pamiec i usuwamy niepotrzebne pociski
	pociski.erase(remove_if(pociski.begin(), pociski.end(),
		[](const unique_ptr<pocisk>& p) { return !p->CzyZywy(); }),
		pociski.end());//tymczasowa zmienna p to kazdy pocisk z wektora pociski jesli p nie zywy to usuwamy go z wektora

}

void KierownikWiezy::zasiegDebug(sf::RenderWindow& window) {
	// Rysuj zasieg wszystkich wiez 
	for (auto& wieza_obiekt : wieze) {
		wieza_obiekt.zasiegDebug(window);
	}
	// Rysuj pociski 
	for (auto& pocisk_obiekt : pociski) {
		pocisk_obiekt->zasiegDebug(window);
	}
}

void KierownikWiezy::DodajWieze(sf::Vector2f pozycja, string typ_wiezy) {
	// Tworzenie callbackow
	FZwrotnaObrazen callbackObrazen = [this](int celId, float ilosc) {
		PrzyznajObrazenia(celId, ilosc);
	};
	FUtworzPocisk callbackPocisk = [this](int wiezaId, int celId, sf::Vector2f pozycjaStartowa, float obrazenia) {
		UtworzPocisk(wiezaId, celId, pozycjaStartowa, obrazenia);
	};
	float zasieg=150.0f;
	float obrazenia=10.0f;
	float czasOdnowienia=1.5f;

	// Dodaj nowa wieze do listy
	wieze.emplace_back(nastepneIdWiezy++,
		pozycja, zasieg,
		obrazenia, czasOdnowienia,
		callbackObrazen, callbackPocisk);
}

// Funkcja callback do przyznawania obrazen gdy pocisk trafia w cel
void KierownikWiezy::PrzyznajObrazenia(int celId, float ilosc) {
	// Tutaj mozna dodac logike przyznawania obrazen celowi o danym ID
	cout << "Cel o ID: " << celId << " otrzymal " << ilosc << " obrazen.\n" << endl;
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