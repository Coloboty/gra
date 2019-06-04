#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"
#include "f_pomocnicze.h"
#include "plansza.h"
#include "ai.h"

using namespace sf;
using namespace std;

void twoPlayerMode(char *name);
void onePlayerMode(char *name, uint depth);

int main(int argc, char *argv[]){
    uint depth;
    char c, r;

    cout << "Wybierz tryb gry: " << endl;
    cout << "1: Z komputerem" << endl;
    cout << "2: Z innym człowiekiem" << endl;
    cout << "Wybór: ";
    cin >> c;

    while(true){
	if(c == '1'){
	    cout << endl;
	    cout << "Wybierz poziom trudności: ";
	    cin >> depth;
	    onePlayerMode(argv[0], depth);
	}
	else if(c == '2'){
	    twoPlayerMode(argv[0]);
	}
	else{
	    cout << "Nie rozumiem." << endl;
	    break;
	}

	cout << endl << endl;
	cout << "Jeszcze raz? (y/n): ";
	cin >> r;

	if(r == 'n')
	    break;
	else if(r == 'y');
	else
	    break;
    }

    return 0;
}

void onePlayerMode(char *name, uint depth){
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), name);
    board plansza;
    CircleShape kropka(DOT_RADIUS);
    Color p1_ghost;
    bool end= false;
    Vector2u ind, dodana;
    ai pepe(2, depth);
    game_state ruch;

    p1_ghost= P1_COLOR;
    p1_ghost.a= 150;

    kropka.setFillColor(p1_ghost);
    
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

		if(!end){
		    if(event.key.code == Keyboard::Right && ind.x < BOARD_SIZE_X-1)
			ind.x++;
		
		    if(event.key.code == Keyboard::Left && ind.x > 0)
			ind.x--;
		
		    /* -------------- */

		    if(event.key.code == Keyboard::Return){
			dodana= plansza.addDot(1, ind.x);
			window.draw(plansza);
			window.display();
			ruch= plansza.state;
		    		    
			if(plansza.checkWin()){
			    cout << "Gracz " << 1 << " wygrywa!" << endl;
			    end= true;
			}
			else{
			    pepe.minMax(ruch);
			    dodana= plansza.addDot(2, ruch.pos.x);

			    if(plansza.checkWin()){
				cout << "Gracz " << 2 << " wygrywa!" << endl;
				end= true;
			    }
			}
		    }
		}
	    }
        }

	
	kropka.setPosition(plansza.getGridCoords(ind).x-kropka.getRadius(), plansza.getGridCoords(ind).y-kropka.getRadius());
		
        window.clear();
	
	window.draw(plansza);
	window.draw(kropka);
	
        window.display();
    }
}

void twoPlayerMode(char *name){
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), name);
    board plansza;
    CircleShape kropka(DOT_RADIUS);
    Color p1_ghost, p2_ghost;
    Vector2u ind, dodana;
    uint gracz= 1;

    p1_ghost= P1_COLOR;
    p1_ghost.a= 150;
    p2_ghost= P2_COLOR;
    p2_ghost.a= 150;

    kropka.setFillColor(p1_ghost);
    
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

		if(event.key.code == Keyboard::Right && ind.x < BOARD_SIZE_X-1)
		    ind.x++;
		
		if(event.key.code == Keyboard::Left && ind.x > 0)
		    ind.x--;
		
		/* -------------- */
		
		if(event.key.code == Keyboard::Return){
		    if(plansza.isValidMove(ind.x)){
			dodana= plansza.addDot(gracz, ind.x);
			/* cout << dodana.x << "   " << dodana.y << endl; */
		    
			if(plansza.checkWin()){
			    cout << "Gracz " << gracz << " wygrywa!" << endl;
			    window.close();
			}
		    
			if(gracz == 1){
			    gracz= 2;
			    kropka.setFillColor(p2_ghost);
			}
			else{
			    gracz= 1;
			    kropka.setFillColor(p1_ghost);
			}
		    }
		}
	    }
        }

	
	kropka.setPosition(plansza.getGridCoords(ind).x-kropka.getRadius(), plansza.getGridCoords(ind).y-kropka.getRadius());
		
        window.clear();
	
	window.draw(plansza);
	window.draw(kropka);
	
        window.display();
    }
}
