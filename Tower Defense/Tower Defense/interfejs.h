#pragma once 
#include <functional>
#include <SFML/System/Vector2.hpp>
using namespace std;
// Definicja typu funkcji callback do przyznawania obrazen

using FZwrotnaObrazen = function<void(int, float)>;
// Definicja typu funkcji callback do tworzenia pociskow
// Przyjmuje pozycje startowa, pozycje celu i obrazenia
//int celid- id celu do ktorego ma leciec pocisk, int wiezaid - id wiezy ktora wystrzelila pocisk,
// sf:: Vector2f pozycjaStartowa - pozycja startowa pocisku, float obrazenia - obrazenia jakie ma zadac pocisk
//float obrazenia - obrazenia jakie ma zadac pocisk

using FUtworzPocisk = function<void(int celId, int wiezaId, sf::Vector2f pozycjaStartowa, float obrazenia)>;