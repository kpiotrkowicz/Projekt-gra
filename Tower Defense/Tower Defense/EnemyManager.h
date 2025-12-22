#pragma once
#include "Enemy.h"
#include <list>

class EnemyManager {
public:
    void setPath(const std::vector<sf::Vector2f>& path);

    void spawnEnemy(int type = 0);
    void spawnWave(int count);

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::list<Enemy> enemies;
    std::vector<sf::Vector2f> path;
};
