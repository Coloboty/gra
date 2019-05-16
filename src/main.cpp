#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "f_pomocnicze.h"
#include "plansza.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

#define BOARD_SIZE_X 10
#define BOARD_SIZE_Y 10

using namespace sf;
using namespace std;

enum player{BLUE, RED};

int main()
{
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "wadowice");
    board plansza(Vector2f(BOARD_SIZE_X, BOARD_SIZE_Y), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    Font czcionka;
    Text tekst, snap, indeks;
    CircleShape kropka(11);
    Vector2i mysz, ind;
    Vector2f smysz;
    
    if(!czcionka.loadFromFile("assets/LinLibertine.ttf")){
	cout << "NIE WCZYTANO CZCIONKI" << endl;
	return 1;
    }

    tekst.setFont(czcionka);
    tekst.setCharacterSize(24);
    tekst.setFillColor(Color::Black);
    tekst.setPosition(20, 10);

    snap.setFont(czcionka);
    snap.setCharacterSize(24);
    snap.setString(to_string(2137));
    snap.setFillColor(Color::Black);
    snap.setPosition(20, 50);

    indeks.setFont(czcionka);
    indeks.setCharacterSize(24);
    indeks.setString(to_string(2137));
    indeks.setFillColor(Color::Black);
    indeks.setPosition(20, 90);

    /* kropka.setOrigin(5,5); */
    kropka.setFillColor(Color(200, 150, 150));
        
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

	    if(event.type == Event::KeyReleased)
		if(event.key.code == Keyboard::Escape)
		    window.close();
        }

	mysz= Mouse::getPosition(window);
	smysz= plansza.snapToGrid(mysz);
	ind= plansza.getGridIndex(smysz);
	tekst.setString("X: " + to_string(mysz.x) + "   Y: " + to_string(mysz.y));
	snap.setString("X: " + to_string(smysz.x) + "   Y: " + to_string(smysz.y));
	indeks.setString("X: " + to_string(ind.x) + "   Y: " + to_string(ind.y));

	kropka.setPosition(smysz.x-kropka.getRadius(), smysz.y-kropka.getRadius());
		
        window.clear();
	
	window.draw(plansza);
	window.draw(tekst);
	window.draw(snap);
	window.draw(indeks);
	window.draw(kropka);
	
        window.display();
    }

    return 0;
}
