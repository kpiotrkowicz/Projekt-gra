#include <SFML/Graphics.hpp>
#include "mapa.h"
#include "hud.h"
#include "forge.h"

bool graStart = false;//zmienna sprawdzajaca czy gra zostala ropoczeta
bool gameOver = false;
int wynik = 0; //zmienna przechowujaca wynik

int main() {
    //sf::Font font;
    //font.loadFromFile("../Assets/fonts/arial.ttf"); //ladujemy czcionke)
    
    sf::Texture menuTexture; //tekstura przechowujaca grafike menu
    sf::Sprite menuSprite; //sprite odpowiedzialny za rysowanie grafiki
    menuTexture.loadFromFile("../Assets/menu/menu.png"); //ladujemy grafike menu
    menuSprite.setTexture(menuTexture); //ustawiamy teksture dla sprite'a
    menuSprite.setPosition(0.f, 0.f); //ustawiamy pozycje sprite na (0,0)

    sf::Texture gameOverTexture; //tekstura przechowujaca grafike game over
    sf::Sprite gameOverSprite; //sprite odpowiedzialny za rysowanie grafiki
    gameOverTexture.loadFromFile("../Assets/gameover/gameover.png"); //ladujemy grafike menu
    gameOverSprite.setTexture(gameOverTexture); //ustawiamy teksture dla sprite'a
    gameOverSprite.setPosition(0.f, 0.f); //ustawiamy pozycje sprite na (0,0)


    sf::FloatRect przyciskStart(450.f, 300.f, 300.f, 80.f);
    //wynik na tle gameover
    /*sf::Text tekstWynik;
    tekstWynik.setFont(font);
    tekstWynik.setCharacterSize(40);
    tekstWynik.setFillColor(sf::Color::White);
    tekstWynik.setPosition(500.f, 400.f);*/

    sf::RenderWindow window(sf::VideoMode(1536, 1024), "SFML dzia³a!");
    initForge(window);//inicjalizuje kuznie
    initHUD(); // inicjalizuje HUD
    loadMapa();// laduje mape
    
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            handleForgeEvent(event);//obsluga kuzni
            
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                graStart = false;
                gameOver = false;
                
            }
            if (!graStart && event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(
                    event.mouseButton.x,
                    event.mouseButton.y
                );
                if (przyciskStart.contains(mousePos)) {
                    graStart = true; //ustawiamy zmienna na true, aby rozpocz¹æ gre
                }
            }
        }
       
        window.clear();
        if (!graStart) {
        window.draw(menuSprite); //rysuje menu
        }
        else if (gameOver) {
            window.draw(gameOverSprite);
        }
        else {
			renderMapa(window); //rysuje mape
            rysujHUD(window);//rysuje hud
            rysujForge(window);//rysuje kuznie
        }
       
        window.display();
    }


    return 0;
}
