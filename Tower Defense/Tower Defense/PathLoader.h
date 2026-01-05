#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>

class PathLoader {
public:
    // Metoda statyczna s³u¿¹ca do wczytywania wspó³rzêdnych œcie¿ki z pliku tekstowego
    static std::vector<sf::Vector2f> loadPath(std::string filename);
};