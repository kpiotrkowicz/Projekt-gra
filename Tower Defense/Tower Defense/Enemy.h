#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int hp);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isDead() const;
    bool reachedEnd() const;

    int getId() const { return id; }
    void takeDamage(int dmg);

private:
    static int NEXT_ID;
    int id;

    std::vector<sf::Vector2f> path;
    int currentPoint = 0;

    sf::CircleShape shape;

    float speed;
    int hp;

    bool reachedBase = false;
};
