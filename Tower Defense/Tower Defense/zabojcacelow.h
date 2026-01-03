#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class ZabojcaCelow {
public:
	ZabojcaCelow() : id(-1), zycie(0.0f), pozycja({ 0.f,0.f }) {}
	ZabojcaCelow(int id, sf::Vector2f pos, float hp) :
		id(id), zycie(hp), pozycja(pos) {
	}
	void obrazeniacelu(float iloscobrazen) {
		zycie -= iloscobrazen;
		if (zycie <= 0.0f) {
			//cout << "Cel o ID: " << id << " zostal zniszczony!" << endl;
		}
		else {
			//cout<<"Cel ID " << id << " otrzymal " << iloscobrazen << " obrazen. Pozostalo HP: " << zycie << endl;
		}
	}
	int id;
	float zycie;
	sf::Vector2f pozycja;
};