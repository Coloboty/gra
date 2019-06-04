#ifndef PLANSZA_H
#define PLANSZA_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"
#include "f_pomocnicze.h"

using namespace std;
using namespace sf;

/* ---------------- */
/* KLASA GAME_STATE */
/* ---------------- */

/* Reprezentuje stan rozgrywki (pozycje/ilość kropek na planszy) */
class game_state{
    /* Pozycja kropek na planszy */
    array< array<uint, BOARD_SIZE_X>, BOARD_SIZE_Y> grid;
public:
    /* Pozycja ostatniej wrzuconej kropki */
    Vector2u pos;
    
    game_state(){
	pos= Vector2u(0, 0);
	
	for(uint i= 0; i < BOARD_SIZE_X; i++){
	    for(uint j= 0; j < BOARD_SIZE_Y; j++)
		grid[i][j]= 0;
	}
    }

    void operator =(game_state &s) {grid= s.grid;}
    
    void setGridState(uint state, Vector2u index) {grid[index.x][index.y]= state;}
    uint getGridState(Vector2u index) const {return grid[index.x][index.y];}
    void printState(void) const;
    bool exists(Vector2u index) const {return !(getGridState(index) == 0);}
    bool isValidMove(uint row) const;
    bool checkWin(void) const {return count(pos, getGridState(pos)) >= 4;}
    bool checkWin(Vector2u index) const {return count(index, getGridState(index)) >= 4;}
    int chase(uint player, Vector2u start, Vector2u dir) const;
    int count(Vector2u index, uint player) const;
    int countAll(Vector2u index, uint player) const;
    
    Vector2u addDot(uint player, uint row);
};

/* ----------- */
/* KLASA BOARD */
/* ----------- */

/* Reprezentuje graficzną stronę planszy */
class board : public Drawable, public Transformable{
    Vector2f screen_size, grid_spacing;
    Vector2u grid_size;
    Color grid_color, board_color;
    uint grid_thickness;

    vector<CircleShape> dot_shapes;
    vector<LineShape> grid_lines;
    
    virtual void draw(RenderTarget &target, RenderStates states) const{
	RectangleShape background(screen_size);
	background.setPosition(getPosition());
	target.draw(background, states);
	
	for(uint i= 0; i < grid_lines.size(); i++)
	    target.draw(grid_lines[i], states);

	for(uint i= 0; i < dot_shapes.size(); i++)
	    target.draw(dot_shapes[i], states);
    }
    
public:
    game_state state;
    
    board();

    Vector2f getGridCoords(Vector2u index);
    bool checkWin(void) {return state.checkWin();}
    bool doesExist(Vector2u index) {return state.exists(index);}
    bool isValidMove(uint row) {return state.isValidMove(row);}
    Vector2u addDot(uint player, uint row);
};

#endif
