#include "Enemy.h"
#include <cmath>

Enemy::Enemy(const std::vector<sf::Vector2f>& p, float s, int hp, const sf::Texture& texture, int m, int d)
    : path(p), speed(s), currentHp(hp), hasReachedEnd(false), money(m), demage(d)
{
    // Przypisanie tekstury i ustawienie punktu obrotu na œrodek grafiki
    sprite.setTexture(texture);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Ustawienie pozycji startowej na pierwszym punkcie œcie¿ki (wspolrzednej)
    if (!path.empty()) {
        sprite.setPosition(path[0]);
        currentPointIndex = 1;
    }
}

void Enemy::update(float dt) {
    // Przerwanie aktualizacji, jeœli przeciwnik zakoñczy³ trasê, zgin¹³ lub œcie¿ka wygas³a
    if (hasReachedEnd || isDead() || currentPointIndex >= path.size()) return;

    sf::Vector2f targetPos = path[currentPointIndex];
    sf::Vector2f currentPos = sprite.getPosition();

    // Obliczanie wektora kierunku i dystansu do nastêpnego punktu
    sf::Vector2f direction = targetPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// Sprawdzanie pozycji celu, jezeli jest blisko, przejdz do kolejnego punktu
    if (distance < 2.0f) {
        currentPointIndex++;
        if (currentPointIndex >= path.size()) {
            hasReachedEnd = true;
        }
    }
    else {
        // Prêdkoœci ruchu 
        sf::Vector2f velocity = (direction / distance) * speed;
        sprite.move(velocity * dt);

        // Obliczanie k¹ta i rotacja postaci w stronê kierunku poruszanie siê
        float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
        sprite.setRotation(angle);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    // Wyœwietlenie grafiki przeciwnika w oknie
    window.draw(sprite);
}