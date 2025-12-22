#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>

class PathLoader {
public:
    static std::vector<sf::Vector2f> loadPath(std::string filename);
};
