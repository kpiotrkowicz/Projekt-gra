#include "KierownikWiezy.h"
#include <iostream>
#include <functional>
#include <list>
#include <string>
#include <algorithm>//zebysmy mogli usuwac pociski z wektora
#include "EnemyManager.h"
using namespace std;
using namespace std::placeholders;


///////////////////nowe dodane 10.01.2026
KierownikWiezy::KierownikWiezy(FZwrotnaObrazen zewnetrznyCallbackObrazen, FUtworzPocisk callbackStworzPocisk)
	: zewnetrznyCallbackObrazen(zewnetrznyCallbackObrazen) {
	// Konstruktor kierownika wiezy
	if (teksturaMenuUlepszen.loadFromFile("../Assets/hud/upgrade_menu.png")) {
		spriteMenuUlepszen.setTexture(teksturaMenuUlepszen);
		sf::FloatRect bounds = spriteMenuUlepszen.getLocalBounds();
		spriteMenuUlepszen.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		//skalujemy do rozmiaru ktory chcemy
		float skala = 40.f / bounds.width; // Przyk³adowa skala
		spriteMenuUlepszen.setScale(skala, skala);
	}
	if (!teksturaMenuUlepszen.loadFromFile("../Assets/hud/upgrade_menu.png")) {
		cout << "KRYTYCZNY BLAD: Nie znaleziono grafiki menu!" << endl;
	}
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
	if (menuWidoczne) {
		window.draw(spriteMenuUlepszen);
	}
}
void KierownikWiezy::reset()
{
	// usuñ wszystkie wie¿e
	wieze.clear();
	pociski.clear();
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
	float obrazenia = 5.0f;
	float czasOdnowienia = 1.5f;

	if (typ == "tower_1") {//wieza lucznikow ta slomiana 
		zasieg = 220.0f;
		obrazenia = 5.0f;
		czasOdnowienia = 0.3f;
	}
	else if (typ == "tower_2") {//wieza ognia ta kamienna z plomieniem 
		zasieg = 160.0f;
		obrazenia = 8.0f;
		czasOdnowienia = 1.8f;
	}
	else if (typ == "tower_3") {//wieza lodowa- krysztalowa
		zasieg = 140.0f;
		obrazenia = 12.0f;
		czasOdnowienia = 1.2f;
	}
	else if (typ == "tower_4") {//wieza magzczna zlota
		zasieg = 190.0f;
		obrazenia = 15.0f;
		czasOdnowienia = 2.5f;
	}
	wieze.emplace_back(nastepneIdWiezy++,
		pozycja, zasieg,
		obrazenia, czasOdnowienia, zewnetrznyCallbackObrazen, bind(&KierownikWiezy::UtworzPocisk, this, _1,_2,_3,_4),typ);
	cout << "Dodano wieze: " << typ << " na pozycji " << pozycja.x << "," << pozycja.y << endl;
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


//zrobione dzi 10.01.2026
void KierownikWiezy::ObsluzKlikniecie(sf::Vector2f mousePos, EnemyManager& manager)
{
	//jesli menu otwarte to sprawdzamy czy kliknieto w ikone ulepszenia
	if (menuWidoczne && wybranaWieza != nullptr) {
		if (spriteMenuUlepszen.getGlobalBounds().contains(mousePos)) {
			//kliknieto w ikone ulepszenia
			float srodekY = spriteMenuUlepszen.getPosition().y;
			if (mousePos.y < srodekY) {
				int koszt = 100 * wybranaWieza->dajPoziom(); //koszt ulepszenia roœnie wraz z poziomem wiezy
				if (manager.getPlayerMoney() >= koszt) {
					if (wybranaWieza->Ulepsz()) {
						manager.moneySum(-koszt); //odejmujemy pieniadze gracza
						cout << "Ulepszono wieze o id: " << wybranaWieza->PobierzId() << " do poziomu: " << wybranaWieza->dajPoziom() << endl;
					}
					else {
						cout << "Wieza o id: " << wybranaWieza->PobierzId() << " jest juz na maksymalnym poziomie ulepszen." << endl;
					}
				}
				else {
					cout << "Brak wystarczajacych srodkow na ulepszenie wiezy o id: " << wybranaWieza->PobierzId() << endl;
				}
			}
			else {
				//kliknieto w dolna czesc ikony- zmniejszamy poziom wiezy
				if (wybranaWieza->ZmniejszPoziom()) {
					int zwrot = 50 * wybranaWieza->dajPoziom(); //zwrot pieniedzy zalezy od poziomu wiezy
					manager.moneySum(zwrot); //dodajemy pieniadze gracza
					cout << "Zmniejszono poziom wiezy o id: " << wybranaWieza->PobierzId() << " do poziomu: " << wybranaWieza->dajPoziom() << endl;
				}
				else {
					cout << "Wieza o id: " << wybranaWieza->PobierzId() << " jest na najnizszym poziomie." << endl;
				}
				//zamykamy menu bez ulepszania
			
				cout << "Zamknieto menu ulepszen bez ulepszania wiezy o id: " << wybranaWieza->PobierzId() << endl;
			}
			menuWidoczne = false;
			wybranaWieza = nullptr;
			return;
		}
	}
	//sprawdzamy czy kliknieto w jakas wieze
	bool kliknietoWWieze = false;
	for (auto& wieza_obiekt : wieze) {
		if (wieza_obiekt.czyKliknieto(mousePos)) {
			wybranaWieza = &wieza_obiekt;
			menuWidoczne = true;
			//ustawiamy pozycje menu nad wieza
			spriteMenuUlepszen.setPosition(wieza_obiekt.PobierzPozycje().x, wieza_obiekt.PobierzPozycje().y - 50.f);
			kliknietoWWieze = true;
			break;
		}
	}
	//kliknieto w puste miejsce na mapie - zamykamy menu
	if (!kliknietoWWieze) {
		menuWidoczne = false;
		wybranaWieza = nullptr;
	}
}

bool KierownikWiezy::kolizjaWiezy(sf::Vector2f pozycja, float promienBlokady) const
{
	for (const auto& w : wieze) {
		sf::Vector2f posWiezy = w.PobierzPozycje(); 

		float dx = pozycja.x - posWiezy.x;
		float dy = pozycja.y - posWiezy.y;
		float dystansKwadrat = (dx * dx) + (dy * dy);

		//czy nowa wie¿a nie jest zbyt blisko postawionej
		if (dystansKwadrat < (promienBlokady * promienBlokady)) {
			return true; // Jest kolizja
		}
	}
	return false; // Brak kolizji
}

