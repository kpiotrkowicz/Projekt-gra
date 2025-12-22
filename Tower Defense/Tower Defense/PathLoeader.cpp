#include "PathLoader.h"
#include <queue>
#include <iostream>

std::vector<sf::Vector2f> PathLoader::loadPath(std::string filename) {
    std::vector<sf::Vector2f> path;
    std::ifstream file(filename);

    // Weryfikacja poprawności otwarcia pliku
    if (!file.is_open()) {
        sf::err() << "Błąd: Nie można otworzyć pliku ścieżki: " << filename << std::endl;
        return path;
    }

    float x, y;
    // Odczytywanie par liczb reprezentujących współrzędne X i Y punktów
    while (file >> x >> y) {
        path.push_back(sf::Vector2f(x, y));
    }

    file.close();
    return path;
}