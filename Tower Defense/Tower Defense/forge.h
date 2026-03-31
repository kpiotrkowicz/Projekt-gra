#pragma once
#include <SFML/Graphics.hpp>
#include "EnemyManager.h"

extern int wybranaWieza;
extern int cenaWiezy[4];

void initForge(sf::RenderWindow& window);
void handleForgeEvent(const sf::Event& event, EnemyManager& manager);
void rysujForge(sf::RenderWindow& window, EnemyManager& manager);