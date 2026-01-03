#include "hud.h"
#include <string>

sf::Font hudFont; //czcionka HUD

sf::Text tekstZycie;
sf::Text tekstZloto;
sf::Text tekstFala;

sf::Texture hudTexture;
sf::Sprite hudSprite;

void initHUD() {
	// ladujemy czcionke i teksture HUD
	hudFont.loadFromFile("../Assets/fonts/font.ttf");
	hudTexture.loadFromFile("../Assets/hud/hud_bg.png");
	hudSprite.setTexture(hudTexture);

	tekstZycie.setFont(hudFont);
    tekstZycie.setCharacterSize(22);
	tekstZycie.setFillColor(sf::Color::White);

	tekstZloto.setFont(hudFont);
	tekstZloto.setCharacterSize(22);
	tekstZloto.setFillColor(sf::Color::Yellow);

	tekstFala.setFont(hudFont);
	tekstFala.setCharacterSize(22);
	tekstFala.setFillColor(sf::Color::Cyan);
}
void rysujHUD(sf::RenderWindow& window, int zycieBazy, int zloto, int fala) {
	float margines = 20.f;

	sf::Vector2u rozmiarOkna = window.getSize();
	sf::FloatRect hudRozmiar = hudSprite.getGlobalBounds();

	float hudX = rozmiarOkna.x - hudRozmiar.width - margines;
	float hudY = margines;

	hudSprite.setPosition(hudX, hudY);

	tekstZycie.setString("Zycie: " + std::to_string(zycieBazy));
	tekstZloto.setString("Zloto: " + std::to_string(zloto));
	tekstFala.setString("Fala: " + std::to_string(fala));

	tekstZycie.setPosition(hudX + 20.f, hudY + 15.f);
	tekstZloto.setPosition(hudX + 20.f, hudY + 45.f);
	tekstFala.setPosition(hudX + 20.f, hudY + 75.f);

	window.draw(hudSprite);
	window.draw(tekstZycie);
	window.draw(tekstZloto);
	window.draw(tekstFala);

}