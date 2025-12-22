#include "EnemyManager.h"
#include <iostream>

EnemyManager::EnemyManager() {
    // £adowanie tekstur z plików 
    if (!texStandard.loadFromFile("../assets/enemy/w1.png")) { /* do ob³sugi b³êdu */ }
    if (!tex2.loadFromFile("../assets/enemy/w2.png")) { /* do ob³sugi b³êdu */ }
    if (!tex3.loadFromFile("../assets/enemy/w3.png")) { /* do ob³sugi b³êdu */ }
}

void EnemyManager::setPath(const std::vector<sf::Vector2f>& p) {
    path = p;
}

void EnemyManager::spawnEnemy(int type) {
    if (path.empty()) return;

    // Definiowanie domyœlnych statystyk
    float speed = 80.f;
    int hp = 100;
    sf::Texture* selectedTexture = &texStandard;

    // parametry na podstawie typu przeciwnika
    switch (type) {
    case 1: // Typ 1
        speed = 150.f;
        hp = 50;
        selectedTexture = &tex2;
        break;
    case 2: // Typ2 
        speed = 40.f;
        hp = 300;
        selectedTexture = &tex3;
        break;
    }

    // Dodanie nowego przeciwnika do listy aktywnych obiektów
    enemies.emplace_back(path, speed, hp, *selectedTexture);

    // Informacje o utworzonej jednostce w konsoli
    std::cout << "[SPAWN] Typ: " << type << " HP: " << hp << " Speed: " << speed << std::endl;
}

void EnemyManager::startWave(int count, float delay, int type) {
    enemiesToSpawn = count;
    spawnDelay = delay;
    spawnTimer = delay; // Ustawienie licznika na wartoœæ opóŸnienia, aby pierwszy spawn by³ natychmiastowy
    waveEnemyType = type;
}

bool EnemyManager::isWaveActive() const {
    // Fala jest aktywna, gdy lista wrogów nie jest pusta lub pozosta³y jednostki do stworzenia
    return !enemies.empty() || enemiesToSpawn > 0;
}

void EnemyManager::update(float dt) {
    // Obs³uga licznika spawnowania kolejnych jednostek w fali
    if (enemiesToSpawn > 0) {
        spawnTimer += dt;
        if (spawnTimer >= spawnDelay) {
            spawnEnemy(waveEnemyType);
            enemiesToSpawn--;
            spawnTimer = 0.0f;
        }
    }

    // Raportowanie stanu jednostek co sekundê 
    static float logTimer = 0;
    logTimer += dt;
    if (logTimer >= 1.0f) {
        if (!enemies.empty()) {
            std::cout << "--- Stan fali --- Aktywnych: " << enemies.size() << std::endl;
        }
        logTimer = 0;
    }

    // Pêtla aktualizuj¹ca stan ka¿dego przeciwnika oraz usuwaj¹ca jednostki nieaktywne
    for (auto it = enemies.begin(); it != enemies.end();) {
        it->update(dt);

        if (it->isDead()) {
            std::cout << "[INFO] Potwor zginal!" << std::endl;
            it = enemies.erase(it);
        }
        else if (it->reachedEnd()) {
            std::cout << "[INFO] Potwor uciekl!" << std::endl;
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}

void EnemyManager::draw(sf::RenderWindow& window) {
    // Renderowanie wszystkich obiektów z listy enemies
    for (auto& e : enemies) e.draw(window);
}