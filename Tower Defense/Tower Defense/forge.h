#pragma once
#include <SFML/Graphics.hpp>

extern int wybranawieza;

void initForge(sf::RenderWindow& window);
void handleForgeEvent(const sf::Event& event);
void rysujForge(sf::RenderWindow& window);