#ifndef PLANSZA_H
#define PLANSZA_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"
#include "f_pomocnicze.h"
/* #include "union_find.hpp" */

using namespace std;
using namespace sf;

class game_state{
    array< array<uint, BOARD_SIZE_X>, BOARD_SIZE_Y> grid;
    /* vector< vector<uint> > grid; */
public:
    game_state(){
	for(uint i= 0; i < BOARD_SIZE_X; i++){
	    for(uint j= 0; j < BOARD_SIZE_Y; j++)
		grid[i][j]= 0;
	}
    }

    friend class move_state;
    void setGridState(uint state, Vector2u index) {grid[index.x][index.y]= state;}
    uint getGridState(Vector2u index) const {return grid[index.x][index.y];}
    bool exists(Vector2u index) const {return !(getGridState(index) == 0);}

    bool isValidMove(uint row) const{
	if(row >= BOARD_SIZE_X)
	    return false;

	if(exists(Vector2u(row, 0))){
	    return false;
	}

	return true;
    }    
    
    Vector2u addDot(uint player, uint row){
	Vector2u index;
	index.x= row;
	index.y= 0;
	/* 'spadaj' kropkę z góry na dół */
	/* cout << index.y << endl; */
	for(uint i= 0; i <= BOARD_SIZE_Y-1; i++){
	    if(exists(Vector2u(row, i+1)) || i == BOARD_SIZE_Y-1){
		index.y= i;
		break;
	    }
	}

	setGridState(player, index);
	/* cout << index.x << "  " << index.y << endl; */
	return index;
    }
    
    /* Liczy, ile kropek w danym kierunku */
    int chase(uint player, Vector2u start, Vector2u dir){
	int count= 0;
	Vector2u check= start;
	check.x+= dir.x;
	check.y+= dir.y;
	if(check.x >= BOARD_SIZE_X || check.y >= BOARD_SIZE_Y)
	    return 0;
	
	while(getGridState(check) == player){
	    count++;
	    check.x+= dir.x;
	    check.y+= dir.y;
	    if(check.x >= BOARD_SIZE_X || check.y >= BOARD_SIZE_Y)
		return count;
	}

	return count;
    }

    int count(Vector2u index){
	uint player= getGridState(index);
	if(player != 1 && player != 2)
	    return 0;

	/* Przygotowanie do szukania sąsiadów */
	int xstart, xstop, ystart, ystop;
	if(index.x > 0) xstart= -1;
	else xstart= 0;
	
	if(index.x < BOARD_SIZE_X-1) xstop= 1;
	else xstop= 0;

	if(index.y > 0) ystart= -1;
	else ystart= 0;

	if(index.y < BOARD_SIZE_Y-1) ystop= 1;
	else ystop= 0;

	/* Szukaj dookoła podanej pozycji */
	Vector2u sindex;
	int total= 0;
	for(int i= xstart; i <= xstop; i++){
	    sindex.x= index.x + i;
	    for(int j= ystart; j <=ystop; j++){
		sindex.y= index.y + j;
		/* Jeśli znaleziono sąsiada takiego samego koloru */
		if(getGridState(sindex) == player && (i != 0 || j != 0)){
		    /* Popatrz w jego stronę i w przeciwną i dodaj liczbę kropek*/
		    total= 1 + chase(player, index, Vector2u(i, j));
		    total+= chase(player, index, Vector2u(i*(-1), j*(-1)));
		}
	    }
	}

	return total;
    }
};

/* -------------------------------------------------------------- */

class board : public Drawable, public Transformable{
    Vector2f screen_size;
    Vector2u grid_size;
    vector<LineShape> grid_lines;
    Vector2f grid_spacing;
    Color grid_color, board_color;
    uint grid_thickness;
    game_state state;
    
    vector<CircleShape> dot_shapes;
    
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
    board();
    ~board() {}

    Vector2f getGridCoords(Vector2u index);
    bool checkWin(Vector2u index) {return (state.count(index) >= WIN_CONDITION);}
    bool doesExist(Vector2u index) {return state.exists(index);}
    bool isValidMove(uint row) {return state.isValidMove(row);}
    Vector2u addDot(uint player, uint row);
    game_state &getGameState(void) {return state;}
    
};

#endif
