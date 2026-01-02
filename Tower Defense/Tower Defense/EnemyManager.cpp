#include "EnemyManager.h"
#include <iostream>

EnemyManager::EnemyManager() {
    // £adowanie tekstur z plików 
    if (!texStandard.loadFromFile("../Assets/enemy/w1.png")) { /* na koncu  do zrobienia obluga bledu bo program sie wykrzacza sam jak jest cos zle */ }
    if (!tex2.loadFromFile("../Assets/enemy/w2.png")) { /* do ob³sugi b³êdu */ }
    if (!tank.loadFromFile("../Assets/enemy/w3.png")) {}
}

//Jak ta oblsuga nie bedzie dzialac to mozna optymistycznie zalozyc ze pliki sa zawsze dostepne 
/*
EnemyManager::EnemyManager() {
   texStandard.loadFromFile("../assets/enemy/w1.png");
   tex2.loadFromFile("../assets/enemy/w2.png");
   tank.loadFromFile("../assets/enemy/w3.png");
}*/


void EnemyManager::setPath(const std::vector<sf::Vector2f>& p) {
    path = p;
}

void EnemyManager::spawnEnemy(int type) {
    if (path.empty()) return;

    // Definiowanie domyœlnych statystyk przeciwnika
    float speed = 80.f;
    int hp = 100;
    int reward = 10;
    int demage = 1;
    sf::Texture* selectedTexture = &texStandard;

    // parametry przeciwnikow
    switch (type) {
    case 1: // Typ 1
        speed = 150.f;
        hp = 50;
        reward = 20;
        selectedTexture = &tex2;
        demage = 1;
        break;
    case 2: // Typ2 
        speed = 40.f;
        hp = 300;
        reward = 30;
        selectedTexture = &tank;
        demage = 2;
        break;
    }

    // Dodanie nowego przeciwnika do listy aktywnych obiektów
    enemies.emplace_back(nextEnemyID++, path, speed, hp, *selectedTexture, reward, demage);

    // Informacje o utworzonej jednostce w konsoli
    std::cout << "[SPAWN] ID: " << nextEnemyID-1 << " Typ: " << type << " HP: " << hp << " Speed: " << speed << std::endl;
}   

void EnemyManager::startWave(const WaveConfig& config) {
    
    spawnDelay = config.delay;
    spawnTimer = config.delay; // Ustawienie licznika na wartoœæ opóŸnienia, aby pierwszy spawn by³ natychmiastowy
    spawnQueue = config.kolejnoscEnemies;
}

bool EnemyManager::isWaveActive() const {
	// Fala jest aktywna, gdy lista wrogów nie jest pusta lub pozosta³y jednostki do stworzenia i gracz ma jeszcze ¿ycie
    return !enemies.empty() || !spawnQueue.empty();
}

void EnemyManager::update(float dt) {
    // Obs³uga licznika spawnowania kolejnych jednostek w fali
    if (!spawnQueue.empty()) {
        spawnTimer += dt;
        if (spawnTimer >= spawnDelay) {
            // Wyci¹gamy typ z pocz¹tku wektora
            int typeToSpawn = spawnQueue.front();
            spawnEnemy(typeToSpawn);

            // Usuwamy ten typ z kolejki
            spawnQueue.erase(spawnQueue.begin());
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
    for (auto przeciwnik = enemies.begin(); przeciwnik != enemies.end();) {
        przeciwnik->update(dt);
  
        if (przeciwnik->isDead()) {
			//gracz dostaje pieniadze za zabicie potwora
			playerMoney += przeciwnik->getReward();
            
            std::cout << "[KASA] przeciwnik byl wart: "<< przeciwnik->getReward() << std::endl;
            std::cout << "[PORTFEL] pieniadze gracza: " << playerMoney << std::endl;
            std::cout << "[INFO] Potwor zginal!" << std::endl;
            przeciwnik = enemies.erase(przeciwnik);
            
        }
        else if (przeciwnik->reachedEnd()) {
			playerHealth -= przeciwnik->getDemage();
            std::cout << "[ZYCIE] zycie gracza: " << playerHealth << std::endl;
            std::cout << "[INFO] Potwor uciekl!" << std::endl;
            przeciwnik = enemies.erase(przeciwnik);


            if(playerHealth <= 0) {
                std::cout << "[GAME OVER] Zycie gracza spadlo do zera!" << std::endl;
			}
        }
        else {
            ++przeciwnik;
        }
    }
}

void EnemyManager::draw(sf::RenderWindow& window) {
    // Renderowanie wszystkich obiektów z listy enemies
    for (auto& e : enemies) e.draw(window);
}


std::vector<Enemy>& EnemyManager::getActiveEnemies() {
    return enemies;
}

// Przeszukuje listê w poszukiwaniu ID i zadaje obra¿enia
void EnemyManager::damageEnemy(int id, float iloscObrazen) {
    for (auto& wrog : enemies) {
		if (wrog.id == id) { //w klasie Enemy jest zmienna id publiczna
            wrog.takeDamage(iloscObrazen);
            break; // Znaleziono i trafiono, mo¿na przerwaæ pêtlê
        }
    }
}


void EnemyManager::MouseClick(sf::Vector2f mousePos) {
    for (auto& enemy : enemies) {
        if (enemy.isClicked(mousePos)) {
            enemy.takeDamage(50); //50 obrazen przy klikniêciu
            std::cout << "[HIT] Potwor oberwal! Pozostalo HP: " << enemy.getHp() << std::endl;

            //aby jedno klikniêcie trafia jednego wroga
            break;
        }
    }
}