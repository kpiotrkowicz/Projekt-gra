#include "PathLoader.h"
#include <queue>
#include <iostream>

std::vector<sf::Vector2f> PathLoader::loadPath(std::string filename) {
    std::vector<sf::Vector2f> path;
    std::ifstream file(filename);

    if (!file.is_open()) {
        // Jeśli nie uda się otworzyć, warto wypisać błąd w konsoli
        sf::err() << "Blad: Nie mozna otworzyc pliku sciezki: " << filename << std::endl;
        return path;
    }

    float x, y;
    // Czytamy dopóki w pliku są pary liczb
    while (file >> x >> y) {
        path.push_back(sf::Vector2f(x, y));
    }

    file.close();
    return path;
}
//{
//    sf::Image img;
//    if (!img.loadFromFile(filename))
//    {
//        std::cout << "ERROR: nie moge zaladowac: " << filename << std::endl;
//    }
//
//
//    int w = img.getSize().x;
//    int h = img.getSize().y;
//
//    std::vector<sf::Vector2i> pixels;
//
//    // 1️⃣ Zbieramy piksele ścieżki
//    for (int y = 0; y < h; y++)
//        for (int x = 0; x < w; x++)
//        {
//            sf::Color c = img.getPixel(x, y);
//            if (c == pathColor)
//                pixels.push_back({ x,y });
//        }
//
//    // Jeśli brak ścieżki → zwróć pustą
//    if (pixels.empty())
//        return {};
//
//    // 2️⃣ Znajdujemy punkt startowy (taki który ma najmniej sąsiadów = końcówka)
//    auto countNeighbours = [&](sf::Vector2i p) {
//        int count = 0;
//        for (int dy = -1; dy <= 1; dy++)
//            for (int dx = -1; dx <= 1; dx++)
//            {
//                if (dx == 0 && dy == 0) continue;
//                sf::Vector2i np = { p.x + dx, p.y + dy };
//                if (std::find(pixels.begin(), pixels.end(), np) != pixels.end())
//                    count++;
//            }
//        return count;
//        };
//
//    sf::Vector2i start = pixels[0];
//    for (auto& p : pixels)
//        if (countNeighbours(p) == 1)
//            start = p;
//
//    // 3️⃣ Budujemy uporządkowaną ścieżkę (ciąg pikseli po kolei)
//    std::vector<sf::Vector2f> path;
//    std::vector<sf::Vector2i> ordered;
//
//    ordered.push_back(start);
//
//    while (ordered.size() < pixels.size())
//    {
//        sf::Vector2i current = ordered.back();
//
//        for (int dy = -1; dy <= 1; dy++)
//            for (int dx = -1; dx <= 1; dx++)
//            {
//                if (dx == 0 && dy == 0) continue;
//
//                sf::Vector2i next = { current.x + dx, current.y + dy };
//
//                if (std::find(pixels.begin(), pixels.end(), next) != pixels.end() &&
//                    std::find(ordered.begin(), ordered.end(), next) == ordered.end())
//                {
//                    ordered.push_back(next);
//                    dy = dx = 2;
//                }
//            }
//    }
//
//    // 4️⃣ Konwersja na floaty (i można przerzedzić co np. 10px żeby nie było milionów punktów)
//    int step = 10;
//    for (int i = 0; i < ordered.size(); i += step)
//        path.push_back(sf::Vector2f(ordered[i].x, ordered[i].y));
//
//    return path;
//}
