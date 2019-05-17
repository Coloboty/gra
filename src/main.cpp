#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "config.h"
#include "f_pomocnicze.h"
#include "plansza.h"
#include "union_find.hpp"

using namespace sf;
using namespace std;

/* enum player{BLUE, RED}; */

int main()
{
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "wadowice");
    board plansza(Vector2u(BOARD_SIZE_X, BOARD_SIZE_Y), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    Font czcionka;
    Text tekst, snap, indeks;
    CircleShape kropka(DOT_RADIUS);
    Vector2i mysz;
    Vector2u ind;
    Vector2f smysz;
    union_find<uint> las;
    
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

	    if(event.type == Event::KeyReleased){
		if(event.key.code == Keyboard::Escape)
		    window.close();
		
		if(event.key.code == Keyboard::Return)
		    plansza.addDot(1, ind);
		
		if(event.key.code == Keyboard::E){
		    if(plansza.doesExist(ind))
			cout << "Kropka" << endl;
		    else
			cout << "Brak kropki" << endl;
		}

		if(event.key.code == Keyboard::S)
		    plansza.listNeighbours(ind);

		if(event.key.code == Keyboard::D)
		    plansza.depthSearch(ind);

		if(event.key.code == Keyboard::A)
		    cout << plansza.getDotAddress(ind) << endl;

		if(event.key.code == Keyboard::I)
		    cout << "X: " << plansza.getDotIndex(ind).x << "  Y: " << plansza.getDotIndex(ind).y << endl;
	    }
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
