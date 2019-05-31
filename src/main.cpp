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

void printGameState(move_state test){
    for(uint i= 0; i < BOARD_SIZE_Y; i++){
	for(uint j= 0; j < BOARD_SIZE_X; j++){
	    cout << test.getGridState(Vector2u(j, i)) << "   ";
	}
	cout << endl;
    }
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
    board p1, p2, p3;
    /* Text t; */
    ai papiesz(2);
    vector<move_state> ruchy;
    uint iterator;
    
    p1.addDot(1, 1);
    p1.addDot(2, 1);
    p1.addDot(1, 2);

    ruchy= papiesz.validMoves(p1.getGameState());
    /* t.setFont(global_font); */
    /* t.setFillColor(Color::Black); */
    /* t.setPosition(40, 80); */
    
    while(window.isOpen()){
	Event event;

	while(window.pollEvent(event)){
	    if(event.type == Event::Closed)
		window.close();
	    
	    if(event.type == Event::KeyReleased){
		if(event.key.code == Keyboard::Escape)
		    window.close();

		if(event.key.code == Keyboard::F)
		    iterator++;

		if(iterator >= ruchy.size())
		    iterator= 0;

		/* t.setString(to_string(papiesz.thonk(ruchy[iterator]))); */
		cout << "Papiesz myśli: " << papiesz.thonk(ruchy[iterator]) << endl;
	    }
	}


	window.clear();
	window.draw(p3);
	visualise(window, p3, ruchy[iterator]);
	/* window.draw(t); */
	/* window.draw(p1); */
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
		}
		
		/* -------------- */
		
		if(event.key.code == Keyboard::Return){
		    dodana= plansza.addDot(gracz, ind.x);
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

	
	kropka.setPosition(plansza.getGridCoords(ind).x-kropka.getRadius(), plansza.getGridCoords(ind).y-kropka.getRadius());
		
        window.clear();
	
	window.draw(plansza);
	window.draw(kropka);
	
        window.display();
    }
}
