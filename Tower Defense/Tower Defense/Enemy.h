#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


class Enemy {
public:
    // Inicjalizacja przeciwnika z okreœlon¹ œcie¿k¹, prêdkoœci¹, punktami ¿ycia i tekstur¹, pieniedzmi obrazeniami
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int hp, const sf::Texture& texture, int m, int d);

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

	// Zadawanie obra¿eñ przeciwnikowi
    void takeDamage(int amount) {
        currentHp -= amount;
        if (currentHp < 0) currentHp = 0;
    }
    // Nagroda za zabicie przeciwnika
	int getReward() const { return money; }
    // Obrazenia zadane graczowi(jak dotrze do konca scie¿ki)
	int getDemage() const { return demage; }

    bool isClicked(sf::Vector2f mousePos) const {
        return sprite.getGlobalBounds().contains(mousePos);
    }


private:
    std::vector<sf::Vector2f> path; // Kontener przechowuj¹cy wspó³rzêdne punktów drogi
    size_t currentPointIndex = 0;    // Indeks aktualnego punktu docelowego na œcie¿ce

    sf::Sprite sprite;      // Obiekt graficzny reprezentuj¹cy przeciwnika
    float speed;            // Szybkoœæ poruszania siê jednostki
    int currentHp;          // Aktualny stan zdrowia
    bool hasReachedEnd = false; // Flaga informuj¹ca o dotarciu do koñca trasy
	int money;		  // Nagroda pieniê¿na za pokonanie przeciwnika
	int demage;      // Obra¿enia zadawane graczowi po dotarciu do koñca trasy
};