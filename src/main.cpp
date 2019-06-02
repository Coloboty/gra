#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "config.h"
#include "f_pomocnicze.h"
#include "plansza.h"
#include "ai.h"

using namespace sf;
using namespace std;

void twoPlayerMode(void);
void onePlayerMode(void);

Font global_font;

bool initText(void){
    return global_font.loadFromFile("assets/LinLibertine.ttf");
}

void visualise(RenderTarget &target, board b, move_state state){
    uint p;
    Vector2f coords;
    Text ruch;
    ruch.setFont(global_font);
    ruch.setPosition(40,40);
    ruch.setFillColor(Color::Black);
    ruch.setString(to_string(state.pos.x));
    target.draw(ruch);

    
    vector<CircleShape> dots;
    for(uint i= 0; i < BOARD_SIZE_X; i++){
	for(uint j= 0; j < BOARD_SIZE_Y; j++){
	    p= state.getGridState(Vector2u(i, j));
	    
	    if(p){
		coords= b.getGridCoords(Vector2u(i, j));
		dots.push_back(CircleShape(DOT_RADIUS));
		dots.back().setPosition(coords.x-DOT_RADIUS, coords.y-DOT_RADIUS);
		if(p == 1) dots.back().setFillColor(P1_COLOR);
		else if(p == 2) dots.back().setFillColor(P2_COLOR);
	    }
	}
    }

    for(uint i= 0; i < dots.size(); i++)
	target.draw(dots[i]);
}

int main(void){
    if(!initText()){
	cout << "Nie udało się załadować czcionki!" << endl;
	return 0;
    }

    onePlayerMode();
    /* twoPlayerMode(); */

    return 0;
}

void onePlayerMode(void){
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "wadowice");
    board plansza;
    CircleShape kropka(DOT_RADIUS);
    Color p1_ghost, p2_ghost;
    bool debug;
    Vector2u ind, dodana;
    uint gracz= 1;
    ai papiesz(2);
    move_state ruch;
    vector<move_state> ruchy;

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

		if(event.key.code == Keyboard::K){
		    if(debug) debug= false;
		    else debug= true;
		}

		if(event.key.code == Keyboard::Right && ind.x < BOARD_SIZE_X-1)
		    ind.x++;
		
		if(event.key.code == Keyboard::Left && ind.x > 0)
		    ind.x--;

		if(event.key.code == Keyboard::Down && ind.y < BOARD_SIZE_Y-1)
		    ind.y++;
		
		if(event.key.code == Keyboard::Up && ind.y > 0)
		    ind.y--;

		if(!debug)
		    ind.y= 0;
		else{

		    if(event.key.code == Keyboard::C){
			if(plansza.checkWin(ind))
			    cout << "wygrana" << endl;
			else
			    cout << "brak" << endl;   
		    }

		    if(event.key.code == Keyboard::R)
			plansza.addDot(1, ind.x);

		    if(event.key.code == Keyboard::B)
			plansza.addDot(2, ind.x);

		    if(event.key.code == Keyboard::F){
			ruch.pos= ind;
			cout << papiesz.thonk(ruch) << endl;
		    }
			
		}
		
		/* -------------- */

		
		
		if(event.key.code == Keyboard::Return){
		    dodana= plansza.addDot(gracz, ind.x);
		    ruch= plansza.getGameState();
		    cout << ruch.count(dodana) << endl;
		    
		    if(plansza.checkWin(dodana)){
			cout << "Gracz " << gracz << " wygrywa!" << endl;
			window.close();
			return;
		    }
		    
		    if(gracz == 1){
			gracz= 2;
			kropka.setFillColor(p2_ghost);

			
			/* ruch.printState(); */
			papiesz.minMax(ruch, 5, 2);
			ind.x= ruch.pos.x;
		    }
		    else{
			gracz= 1;
			kropka.setFillColor(p1_ghost);
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

void twoPlayerMode(void){
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "wadowice");
    board plansza;
    CircleShape kropka(DOT_RADIUS);
    Color p1_ghost, p2_ghost;
    bool debug;
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

		if(event.key.code == Keyboard::K){
		    if(debug) debug= false;
		    else debug= true;
		}

		if(event.key.code == Keyboard::Right && ind.x < BOARD_SIZE_X-1)
		    ind.x++;
		
		if(event.key.code == Keyboard::Left && ind.x > 0)
		    ind.x--;

		if(event.key.code == Keyboard::Down && ind.y < BOARD_SIZE_Y-1)
		    ind.y++;
		
		if(event.key.code == Keyboard::Up && ind.y > 0)
		    ind.y--;

		if(!debug)
		    ind.y= 0;
		else{

		    if(event.key.code == Keyboard::C){
			if(plansza.checkWin(ind))
			    cout << "wygrana" << endl;
			else
			    cout << "brak" << endl;   
		    }

		    if(event.key.code == Keyboard::R)
			plansza.addDot(1, ind.x);

		    if(event.key.code == Keyboard::B)
			plansza.addDot(2, ind.x);

		    if(event.key.code == Keyboard::F)
			cout << plansza.getGameState().getGridState(ind) << endl;

		    if(event.key.code == Keyboard::G)
			plansza.getGameState().setGridState(1, ind);
		}
		
		/* -------------- */
		
		if(event.key.code == Keyboard::Return){
		    if(plansza.isValidMove(ind.x)){
			dodana= plansza.addDot(gracz, ind.x);
			/* cout << dodana.x << "   " << dodana.y << endl; */
		    
			if(plansza.checkWin(dodana)){
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
