#pragma once
#include "Enemy.h"
#include <list>

struct WaveConfig //struktura odpowiada za mieszanie potworow w fali
{
    std::vector<int> kolejnoscEnemies; // w jakiej kolejnosci vbeda tworzone potwory - np.{0,0,1,0,0,1}    
    float delay;
};


class EnemyManager {
public:
    EnemyManager();

    // Ustawienie globalnej œcie¿ki dla nowo tworzonych przeciwników
    void setPath(const std::vector<sf::Vector2f>& path);

    // Tworzenie pojedynczej instancji przeciwnika okreœlonego typu
    void spawnEnemy(int type = 0);

    // Konfiguracja parametrów nowej fali (liczba wrogów, odstêp czasowy, typ)
    void startWave(const WaveConfig& config);

    // Sprawdzenie, czy na mapie znajduj¹ siê przeciwnicy lub czy trwa proces spawnowania
    bool isWaveActive() const;

    // Aktualizacja wszystkich aktywnych przeciwników i logiki fal
    void update(float dt);

    // Rysowanie wszystkich ¿ywych przeciwników
    void draw(sf::RenderWindow& window);

    //ile pieniedzy ma gracz
    int getPlayerMoney() const { return playerMoney; }

	//koniec gry gdy zycie spadnie do zera
	bool gameOver() const { return playerHealth <= 0; }
    


    //do testu to klikanie myszy
    void MouseClick(sf::Vector2f mousePos);

private:
    std::vector<int> spawnQueue; // Kolejka typów do spawnowania w obecnej fali
	WaveConfig currentWave;      // Bie¿¹ca konfiguracja fali
    std::list<Enemy> enemies;       // Lista przechowuj¹ca aktywnych przeciwników
    std::vector<sf::Vector2f> path; // Kopia œcie¿ki poruszania siê

	// Zasoby gry
	int playerMoney = 100; //pieniadze gracza
	int playerHealth = 1; //zycie gracza

    // Zmienne steruj¹ce mechanik¹ fal
    int waveEnemyType = 0;
    int enemiesToSpawn = 0;
    float spawnTimer = 0.0f;
    float spawnDelay = 0.5f;

    // Przechowywanie zasobów tekstur w pamiêci managera
    sf::Texture texStandard;
    sf::Texture tex2;
    sf::Texture tank;
};