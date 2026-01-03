#include "wieza.h"
#include <cmath>
#include <iostream>
#include <limits> //inf o cechach typow numerycznych


using namespace std;

// Konstruktor wiezy
//:: operator zakresla, ze funkcja nalezy do klasy
//:id(id) - inicjalizacja pola id klasy wieza wartoscia id przekazana do konstruktora
wieza::wieza(int id, sf::Vector2f pozycja, float zasieg, float obrazenia, float czasOdnowienia, FZwrotnaObrazen callback, FUtworzPocisk callbackPocisk, string typ)
	: id(id), pozycja(pozycja), zasieg(zasieg), obrazenia(obrazenia), czasOdnowienia(czasOdnowienia),
	czasOdOstatniegoStrzalu(0.0f), aktualnyCelId(-1),
	przyznajObrazeniaCallback(callback),
	utworzPociskCallback(callbackPocisk),
	typTargetowania("najblizszy"),// Domyslny typ targetowania
	poziom(1),typ(typ)
{
	if (tekstura.loadFromFile(typ+".png")) {
		sprite.setTexture(tekstura);
		sf::FloatRect b = sprite.getLocalBounds();
		sprite.setOrigin(b.width / 2.f, b.height / 2.f);
		sprite.setPosition(pozycja);
		//skalujemy do rozmiaru ktory chcemy
		float skala = 80.f / b.width; // Przyk³adowa skala
		sprite.setScale(skala, skala);
	}
	cout << "Wieza utworzona o ID: " << id << " na pozycji (" << pozycja.x << ", " << pozycja.y << ")\n"<<endl;
}
//Aktualizacja wiezy wywolywana w kazdej klatce gry przez kierownika wie¿y

void wieza::Aktualizuj(float czasDelta, const vector<Cel>& potencjalneCele)
{//arg potencjalne cele - lista celow do sprawdzenia
	// Aktualizuj czas od ostatniego strzalu
	czasOdOstatniegoStrzalu += czasDelta;
	// Sprawdz czy mamy cel
	//aktualnyCelId = -1; // Resetuj cel na poczatku aktualizacji
	ZnajdzCel(potencjalneCele);

		/*if (aktualnyCelId != -1  && czasOdOstatniegoStrzalu >= czasOdnowienia)
	{
		Strzel();
		czasOdOstatniegoStrzalu = 0.0f; // Resetuj timer strzalu
	}*/
	//strzelaj jesli czas odnow minal 
	if (aktualnyCelId!=-1 && czasOdOstatniegoStrzalu >= czasOdnowienia) {
		Strzel();
		czasOdOstatniegoStrzalu = 0.0f;
	}
	if (aktualnyCelId != -1)
	{
		for (const auto& cel : potencjalneCele) {
			//ZnajdzCel(potencjalneCele);
			if (cel.id == aktualnyCelId) {
				float dx = cel.pozycja.x - pozycja.x;
				float dy = cel.pozycja.y - pozycja.y;
				float kat = atan2(dy, dx) * 180 / 3.14159f;
				sprite.setRotation(kat + 90);//90 zaelzy od tego jak narysowany jest obrazek png
				break;
			}
		
		}
		
	}
	
}

//jak znajdowac cele - szukamy najblizszego celu

void wieza::ZnajdzCel(const vector<Cel>& potencjalneCele)
{

	aktualnyCelId = -1;// Domyslnie brak celu
	//float najblizszyDystansKw = numeric_limits<float>::max(); // tu uzywamy tego limits - Inicjalizacja na maksymalna wartosc float
	float zasiegkw = zasieg * zasieg; // Porownujemy kwadraty dystansow, aby uniknac sqrt
	if (typTargetowania == "najblizszy") {
		float najblizszyDystansKw = numeric_limits<float>::max(); // tu uzywamy tego limits - Inicjalizacja na maksymalna wartosc float
		for (const auto& cel : potencjalneCele)
		{
			// Oblicz dystans do celu 
			float dx = cel.pozycja.x - pozycja.x;
			float dy = cel.pozycja.y - pozycja.y;
			float dystanskw = dx * dx + dy * dy;
			// Sprawdz czy cel jest w zasiegu
			if (dystanskw <= zasiegkw && dystanskw < najblizszyDystansKw) {
				najblizszyDystansKw = dystanskw;
				aktualnyCelId = cel.id;
			}
		}
	}
	else if (typTargetowania == "najsilniejszy") {
		float maxZdrowie = -1.0f; // dodanie minimalnej wartosci
		for (const auto& cel : potencjalneCele)
		{
			// Oblicz dystans do celu 
			float dx = cel.pozycja.x - pozycja.x;
			float dy = cel.pozycja.y - pozycja.y;
			// Sprawdz czy cel jest w zasiegu
			if (dx * dx + dy * dy <= zasiegkw && cel.zdrowie > maxZdrowie) {
				maxZdrowie = cel.zdrowie;
				aktualnyCelId = cel.id;
			}
		}
	}
}
	/*for (const auto& cel : potencjalneCele)
	{
		// Oblicz dystans do celu 
		float dx = cel.pozycja.x - pozycja.x;
		float dy = cel.pozycja.y - pozycja.y;
		float dystanskw = dx * dx + dy * dy;
		// Sprawdz czy cel jest w zasiegu

		if (dystanskw <= zasiegkw && dystanskw < najblizszyDystansKw) {
			najblizszyDystansKw = dystanskw;
			aktualnyCelId = cel.id;
		}


	}*/


//jak strzelac - wywolujemy callback do tworzenia pocisku

void wieza::Strzel()
{
	// Tutaj wywolujemy callback do tworzenia pocisku
	// Przekazujemy ID wiezy, ID celu, pozycje startowa pocisku (pozycja wiezy) i obrazenia
	if (aktualnyCelId != -1&& utworzPociskCallback) // Upewnij sie, ze mamy cel
	{
		cout << "Wieza " << id << " strzelila do celu " << aktualnyCelId << endl;
		utworzPociskCallback(id, aktualnyCelId, pozycja, obrazenia);
		
	}
}

void wieza::zasiegDebug(sf::RenderWindow& window)
{
	// Rysuj zasieg wiezy jako okrag
	sf::CircleShape okrag(zasieg);
	okrag.setFillColor(sf::Color(100, 100, 100, 50)); // Przezroczyste w srodku
	okrag.setOrigin(zasieg, zasieg); // Ustawienie srodka okregu
	okrag.setPosition(pozycja); // Ustawienie pozycji na srodek wiezy
	window.draw(okrag);

	window.draw(sprite);
}

bool wieza::Ulepsz()
{
	if (poziom < MAKSYMPOZIOM)
	{
		poziom++;
		zasieg *= 1.5f; // Zwieksz zasieg o 20%
		obrazenia *= 1.5f; // Zwieksz obrazenia o 50%
		czasOdnowienia *= 0.9f; // Zmniejsz czas odnowienia o 10%
		cout << "Wieza " << id << " ulepszona do poziomu " << poziom << endl;
		return true;
	}
	else
	{
		cout << "Wieza " << id << " jest juz na maksymalnym poziomie!" << endl;
		return false;
	}
}