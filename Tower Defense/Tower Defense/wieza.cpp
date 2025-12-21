#include "wieza.h"
#include <cmath>
#include <iostream>
#include <limits> //inf o cechach typow numerycznych

using namespace std;

// Konstruktor wiezy
//:: operator zakresla, ze funkcja nalezy do klasy
//:id(id) - inicjalizacja pola id klasy wieza wartoscia id przekazana do konstruktora
wieza:: wieza(int id, sf::Vector2f pozycja, float zasieg, float obrazenia, float czasOdnowienia, FZwrotnaObrazen callback, FUtworzPocisk callbackPocisk)
	: id(id), pozycja(pozycja), zasieg(zasieg), obrazenia(obrazenia), czasOdnowienia(czasOdnowienia),
	  czasOdOstatniegoStrzalu(0.0f), aktualnyCelId(-1),
	  przyznajObrazeniaCallback(callback),
	  utworzPociskCallback(callbackPocisk),
	  typTargetowania("najblizszy"),aktualnyCelId(-1) // Domyslny typ targetowania
{
	cout << "Wieza utworzona o ID: " << id << " na pozycji (" << pozycja.x << ", " << pozycja.y << ")\n"<<endl;
}
//Aktualizacja wiezy wywolywana w kazdej klatce gry przez kierownika wie¿y

void wieza::Aktualizuj(float czasDelta, const vector<Cel>& potencjalneCele)
{//arg potencjalne cele - lista celow do sprawdzenia
	// Aktualizuj czas od ostatniego strzalu
	czasOdOstatniegoStrzalu += czasDelta;
	// Sprawdz czy mamy cel
	if (aktualnyCelId == -1)
	{
		ZnajdzCel(potencjalneCele);
	}
	// Jesli mamy cel, sprawdz czy mozemy strzelic
	if (aktualnyCelId != -1)
	{
		if (czasOdOstatniegoStrzalu >= czasOdnowienia)
		{
			Strzel();
			czasOdOstatniegoStrzalu = 0.0f; // Resetuj timer strzalu
		}
	}
}

//jak znajdowac cele - szukamy najblizszego celu

void wieza::ZnajdzCel(const vector<Cel>& potencjalneCele)
{
	
	aktualnyCelId = -1;// Domyslnie brak celu
	float najblizszyDystansKw = numeric_limits<float>::max(); // tu uzywamy tego limits - Inicjalizacja na maksymalna wartosc float
	float zasiegkw = zasieg * zasieg; // Porownujemy kwadraty dystansow, aby uniknac sqrt

	for (const auto& cel : potencjalneCele)
	{
		// Oblicz dystans do celu 
		float dx = cel.pozycja.x - pozycja.x;
		float dy = cel.pozycja.y - pozycja.y;
		float dystanskw = dx * dx + dy * dy;
		// Sprawdz czy cel jest w zasiegu
		
		if (dystanskw <= zasiegkw && zasiegkw < najmniejszyDystansKwadrat) {
			najblizszyDystansKw = dystanskw;
			aktualnyCelId = cel.id;
		}
			
		
	
	
}

//jak strzelac - wywolujemy callback do tworzenia pocisku

void wieza::Strzel()
{
	// Tutaj wywolujemy callback do tworzenia pocisku
	// Przekazujemy ID wiezy, ID celu, pozycje startowa pocisku (pozycja wiezy) i obrazenia
	if (aktualnyCelId != -1&& utrworzPociskCallback) // Upewnij sie, ze mamy cel
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

	//rysunek samej wiezy
	sf::RectangleShape towershape({ 20.0f,40.0f });
	towerShape.setOrigin(10.0f, 20.0f); // Srodek wiezy
	towerShape.setPosition(pozycja);
	towerShape.setFillColor(sf::Color::Green);
	window.draw(towerShape);
}