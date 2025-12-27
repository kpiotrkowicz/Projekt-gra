#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


class Enemy {
public:
    // Inicjalizacja przeciwnika z okreœlon¹ œcie¿k¹, prêdkoœci¹, punktami ¿ycia i tekstur¹
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int hp, const sf::Texture& texture);

    // Aktualizacja stanu logicznego obiektu w ka¿dej klatce
    void update(float dt);

    // Renderowanie przeciwnika w oknie gry
    void draw(sf::RenderWindow& window);

    // Weryfikacja, czy punkty ¿ycia spad³y do zera 
    bool isDead() const { return currentHp <= 0; }

    // Sprawdzenie, czy przeciwnik ukoñczy³ ca³¹ dostêpn¹ œcie¿kê
    bool reachedEnd() const { return hasReachedEnd; }

    // Pobranie aktualnej liczby punktów ¿ycia
    int getHp() const { return currentHp; }

private:
    std::vector<sf::Vector2f> path; // Kontener przechowuj¹cy wspó³rzêdne punktów drogi
    size_t currentPointIndex = 0;    // Indeks aktualnego punktu docelowego na œcie¿ce

    sf::Sprite sprite;      // Obiekt graficzny reprezentuj¹cy przeciwnika
    float speed;            // Szybkoœæ poruszania siê jednostki
    int currentHp;          // Aktualny stan zdrowia
    bool hasReachedEnd = false; // Flaga informuj¹ca o dotarciu do koñca trasy
};