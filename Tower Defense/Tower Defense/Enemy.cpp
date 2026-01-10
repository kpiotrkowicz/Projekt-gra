#include "Enemy.h"
#include <cmath>

// Definicje statycznych pól
sf::Texture Enemy::sharedHeartTexture;
bool Enemy::sharedHeartTextureLoaded = false;

Enemy::Enemy(int id, const std::vector<sf::Vector2f>& p, float s, int hp, const sf::Texture& texture, int m, int d)
    : id(id), path(p), speed(s), currentHp(hp), maxHp(hp), hasReachedEnd(false), money(m), demage(d)
{
    // Przypisanie tekstury i ustawienie punktu obrotu na œrodek grafiki
    sprite.setTexture(texture);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setScale(0.1f, 0.1f);


    // Ustawienie pozycji startowej na pierwszym punkcie œcie¿ki (wspolrzednej)
    if (!path.empty()) {
        sprite.setPosition(path[0]);
        currentPointIndex = 1;
    }

    // Wczytaj teksturê serca raz (jeœli jeszcze nie wczytano) i ustaw sprite
    if (!Enemy::sharedHeartTextureLoaded) {
        if (Enemy::sharedHeartTexture.loadFromFile("../Assets/enemy/heart.png")) {
            Enemy::sharedHeartTextureLoaded = true;
        }
        else {
            // Niepowodzenie wczytania — mo¿esz dodaæ logowanie tutaj
            Enemy::sharedHeartTextureLoaded = false;
        }
    }

    if (Enemy::sharedHeartTextureLoaded) {
        heartSprite.setTexture(Enemy::sharedHeartTexture);
        heartSprite.setScale(0.05f, 0.05f);
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

        
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    // Wyœwietlenie grafiki przeciwnika w oknie
    window.draw(sprite);
    drawHearts(window);
}
void Enemy::drawHearts(sf::RenderWindow& window)
{
    if (isDead()) return;

    float hpPercent = (float)currentHp / (float)maxHp;

    int heartsToDraw = 0;

    if (hpPercent > 0.66f) {
        heartsToDraw = 3;
    }
    else if (hpPercent > 0.33f) {
        heartsToDraw = 2;
    }
    else if (hpPercent > 0.0f) {
        heartsToDraw = 1;
    }

    sf::Vector2f pos = sprite.getPosition();
    float startX = pos.x - (heartsToDraw * 10.f) / 2.f;
    float y = pos.y - 30.f;

    for (int i = 0; i < heartsToDraw; i++) {
        heartSprite.setPosition(startX + i * 10.f, y);
        window.draw(heartSprite);
    }
}
