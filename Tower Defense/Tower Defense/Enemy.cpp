#include "Enemy.h"
#include <cmath>

int Enemy::NEXT_ID = 0;

Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, int hp)
    : path(path), speed(speed), hp(hp)
{
    id = NEXT_ID++;
    shape.setRadius(12.f);
    shape.setOrigin(12.f, 12.f);
    shape.setFillColor(sf::Color::Red);

    if (!path.empty())
        shape.setPosition(path[0]);
}

void Enemy::update(float dt)
{
    if (currentPoint >= path.size() - 1) {
        reachedBase = true;
        return;
    }

    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f target = path[currentPoint + 1];

    sf::Vector2f dir = target - pos;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length < 1.0f) {
        currentPoint++;
        return;
    }

    dir /= length;
    shape.move(dir * speed * dt);
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

bool Enemy::isDead() const
{
    return hp <= 0;
}

bool Enemy::reachedEnd() const
{
    return reachedBase;
}

void Enemy::takeDamage(int dmg)
{
    hp -= dmg;
}
