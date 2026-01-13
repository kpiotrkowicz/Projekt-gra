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
	// Powi¹zanie z cenami z forge.cpp
	if (typ == "tower_1")      kosztUlepszenia = 50;
	else if (typ == "tower_2") kosztUlepszenia = 100;
	else if (typ == "tower_3") kosztUlepszenia = 200;
	else if (typ == "tower_4") kosztUlepszenia = 350;
	cenaSprzedazy = static_cast<int>(kosztUlepszenia * 0.75f); // 75% kosztu ulepszenia jako cena sprzedazy
	// Za³aduj teksturê wie¿y na podstawie typu
	if (tekstura.loadFromFile("../Assets/hud/" + typ + ".png")) {
		sprite.setTexture(tekstura);
		sf::FloatRect b = sprite.getLocalBounds();
		sprite.setOrigin(b.width / 2.f, b.height / 2.f);
		sprite.setPosition(pozycja);
		//skalujemy do rozmiaru ktory chcemy
		float skala = 80.f / b.width; // Przyk³adowa skala
		sprite.setScale(skala, skala);
	}
	cout << "Wieza utworzona o ID: " << id << " na pozycji (" << pozycja.x << ", " << pozycja.y << ")\n"<<endl;
	for (int i = 0; i < MAKSYMPOZIOM; ++i) {
		string sciezka = "../Assets/hud/lvl_" + to_string(i + 1) + ".png";
		if (!teksturyPoziomow[i].loadFromFile(sciezka)) {
			cout << "Nie mozna zaladowac tekstury poziomu: " << sciezka << endl;
		}
	}
	spritePoziom.setTexture(teksturyPoziomow[0]);
	float skala = 60.f / spritePoziom.getLocalBounds().width; // Przyk³adowa skala
	spritePoziom.setScale(skala, skala);

	sf::FloatRect bounds = spritePoziom.getLocalBounds();
	spritePoziom.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	cout << "Wieza poziom " << poziom << " ustawiona.\n" << endl;

	if (czcionka.loadFromFile("../Assets/fonts/font.ttf")) {
		informacyjny.setFont(czcionka);
		informacyjny.setCharacterSize(20);
		informacyjny.setFillColor(sf::Color::Red);
		informacyjny.setOutlineColor(sf::Color::Black);
		informacyjny.setOutlineThickness(1.5f);
	}

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
				break;
			}
		
		}
		
	}
	if (czasWyswietlaniatekstu > 0.0f) {
		czasWyswietlaniatekstu -= czasDelta; // Zmniejsz czas wyswietlania tekstu
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
	//window.draw(okrag);

	window.draw(sprite);

	//ikona poziomu obok wiezy
	spritePoziom.setPosition(pozycja.x + 40.f, pozycja.y - 20.f);
	window.draw(spritePoziom);

	//rysuuje max/min poziom nad wiweza jesli jest taka potzreaba
	if (czasWyswietlaniatekstu > 0.0f)
	{
		informacyjny.setPosition(pozycja.x - 60.f, pozycja.y - 80.f);
		window.draw(informacyjny);
	}

}

bool wieza::Ulepsz(int& portfel)
{
	int koszt = kosztUlepszenia * poziom; // Koszt ulepszenia zalezy od obecnego poziomu
	if (poziom < MAKSYMPOZIOM)
	{
		if (portfel >= koszt) {
			portfel -= koszt;


			poziom += 1;
			spritePoziom.setTexture(teksturyPoziomow[poziom - 1]);
			zasieg *= 1.15f; // Zwieksz zasieg o 15%
			obrazenia *= 1.3f; // Zwieksz obrazenia o 30%
			czasOdnowienia *= 0.9f; // Zmniejsz czas odnowienia o 10%
			cout << "Wieza " << id << " ulepszona do poziomu " << poziom << endl;
			return true;
		}
		else//brak pieniedzy
		{
			informacyjny.setString("BRAK ZLOTA! WYMAGANE:" + to_string(koszt));
			czasWyswietlaniatekstu = 2.0f; // Napis bêdzie widoczny przez 2 sekundy
			return false;
		}
	}

	else {//limit poziomow 
				informacyjny.setString("MAKSYMALNY POZIOM!");
		czasWyswietlaniatekstu = 2.0f; // Napis bêdzie widoczny przez 2 sekundy
		return false;
	}
	}

bool wieza::ZmniejszPoziom(int& portfel)
{
	if (poziom>1)
	{
		int zwrotZaPoziom = (kosztUlepszenia * (poziom - 1)) / 2;
		portfel += zwrotZaPoziom;
		poziom-=1;
		spritePoziom.setTexture(teksturyPoziomow[poziom - 1]);
		zasieg /= 1.15f; // Zwieksz zasieg o 15%
		obrazenia /= 1.3f; // Zwieksz obrazenia o 30%
		czasOdnowienia /= 0.9f; // Zmniejsz czas odnowienia o 10%
		//resetujemy flage usuniecia wiezy
		czekajNaUsuniecie = false;
		cout << "Wieza " << id << " ulepszona do poziomu " << poziom << endl;

		return true;
	}
	else
	{
		if (!czekajNaUsuniecie) {
			//pierwsze klikniecoe to tylko informacja o tym ze poziom minimalny
			informacyjny.setString("POZIOM MINIMALNY!\n (jesli chcesz usunac wieze nacisnij jeszcze raz)");
			czasWyswietlaniatekstu = 2.0f; // Napis bêdzie widoczny przez 2 sekundy
			czekajNaUsuniecie = true;
			return false;
		}
		else {
			//drugie klikniecie usuniecie i zwrot pieniedzy
			portfel += cenaSprzedazy;
			doUsuniecia = true;//zniekniecie wiezy przez kierownika wiez
			cout << "Wieza " << id << " zostala sprzedana. Zwrot: " << cenaSprzedazy << endl;	
			return true;
		}
		
	}
}