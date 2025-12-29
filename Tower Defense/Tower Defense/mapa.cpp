#include <SFML/Graphics.hpp>
#include <iostream>


sf::Texture mapaTexture;
sf::Sprite mapaSprite;
bool mapaZaladowana = false;

// Ladowanie mapy 
void loadMapa() {
    if (!mapaTexture.loadFromFile("..Assets/maps/mapka.png")) {
        std::cout << "BLAD: Nie mozna zaladowac mapy!\n";
        return;
    }

    mapaSprite.setTexture(mapaTexture);
    mapaZaladowana = true;
}

// Rysowanie mapy
void renderMapa(sf::RenderWindow& window) {
    if (mapaZaladowana) {
        window.draw(mapaSprite);
    }
}



