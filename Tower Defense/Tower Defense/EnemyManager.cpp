#include "EnemyManager.h"

void EnemyManager::setPath(const std::vector<sf::Vector2f>& p)
{
    path = p;
}

void EnemyManager::spawnEnemy(int)
{
    enemies.emplace_back(path, 80.f, 100);
}

void EnemyManager::spawnWave(int count)
{
    for (int i = 0; i < count; i++)
        spawnEnemy();
}

void EnemyManager::update(float dt)
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        it->update(dt);

        if (it->isDead() || it->reachedEnd())
            it = enemies.erase(it);
        else
            ++it;
    }
}

void EnemyManager::draw(sf::RenderWindow& window)
{
    for (auto& e : enemies)
        e.draw(window);
}
