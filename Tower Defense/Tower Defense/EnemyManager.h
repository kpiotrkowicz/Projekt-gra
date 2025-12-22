#pragma once
#include "Enemy.h"
#include <list>

class EnemyManager {
public:
    EnemyManager();

    // Ustawienie globalnej œcie¿ki dla nowo tworzonych przeciwników
    void setPath(const std::vector<sf::Vector2f>& path);

    // Tworzenie pojedynczej instancji przeciwnika okreœlonego typu
    void spawnEnemy(int type = 0);

    // Konfiguracja parametrów nowej fali (liczba wrogów, odstêp czasowy, typ)
    void startWave(int count, float delay, int type);

    // Sprawdzenie, czy na mapie znajduj¹ siê przeciwnicy lub czy trwa proces spawnowania
    bool isWaveActive() const;

    // Aktualizacja wszystkich aktywnych przeciwników i logiki fal
    void update(float dt);

    // Rysowanie wszystkich ¿ywych przeciwników
    void draw(sf::RenderWindow& window);

private:
    std::list<Enemy> enemies;       // Lista przechowuj¹ca aktywnych przeciwników
    std::vector<sf::Vector2f> path; // Kopia œcie¿ki poruszania siê

    // Zmienne steruj¹ce mechanik¹ fal
    int waveEnemyType = 0;
    int enemiesToSpawn = 0;
    float spawnTimer = 0.0f;
    float spawnDelay = 0.5f;

    // Przechowywanie zasobów tekstur w pamiêci managera
    sf::Texture texStandard;
    sf::Texture tex2;
    sf::Texture tex3;
};