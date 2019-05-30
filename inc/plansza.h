#ifndef PLANSZA_H
#define PLANSZA_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include "config.h"
#include "f_pomocnicze.h"
/* #include "union_find.hpp" */

using namespace std;
using namespace sf;

class game_state{
    uint grid[BOARD_SIZE_X][BOARD_SIZE_Y];
public:
    game_state(){
	for(uint i= 0; i < BOARD_SIZE_X; i++)
	    for(uint j= 0; j < BOARD_SIZE_Y; j++)
		grid[i][j]= 0;
    }
    
    void setGridState(uint state, Vector2u index) {grid[index.x][index.y]= state;}
    uint getGridState(Vector2u index) {return grid[index.x][index.y];}
    bool exists(Vector2u index) {return !(getGridState(index) == 0);}
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

    /* Liczy, ile kropek w danym kierunku */
    uint chase(uint player, Vector2u start, Vector2u dir){
	uint count= 0;
	Vector2u check= start;
	check.x+= dir.x;
	check.y+= dir.y;
	if(check.x >= BOARD_SIZE_X || check.y >= BOARD_SIZE_Y)
	    return 0;
	
	while(state.getGridState(check) == player){
	    count++;
	    check.x+= dir.x;
	    check.y+= dir.y;
	    if(check.x >= BOARD_SIZE_X || check.y >= BOARD_SIZE_Y)
		return count;
	}

	return count;
    }
    
public:
    board(const Vector2f &screenSize);
    ~board() {}

    Vector2f getGridCoords(Vector2u index);
    Vector2f getGridSpacing(void) {return grid_spacing;}
    bool doesExist(Vector2u index) {return state.exists(index);}
    
    bool isValidMove(uint player, uint row){
	if(row > grid_size.x)
	    return false;

	if(state.exists(Vector2u(row, 0)) == true){
	    return false;
	}

	return true;
    }    
    
    Vector2u addDot(uint player, uint row){
	Vector2u index;
	index.x= row;
	
	/* Sprawdź, czy ruch jest dozwolony */
	if(!isValidMove(player, index.x))
	    /* return Vector2u(BOARD_SIZE_X, BOARD_SIZE_Y); */
	    return Vector2u(-1, -1);

	/* 'spadaj' kropkę z góry na dół */
	for(uint i= 0; i <= grid_size.y; i++){
	    if(state.exists(Vector2u(row, i+1)) || i == grid_size.y){
		index.y= i;
		break;
	    }
	}
	
	/* Dodaj graficzną reprezentację do listy rzeczy do wyświetlania */
	Vector2f coords= getGridCoords(index);
	dot_shapes.push_back(CircleShape(DOT_RADIUS));
	dot_shapes.back().setPosition(coords.x-DOT_RADIUS, coords.y-DOT_RADIUS);
	if(player == 1)
	    dot_shapes.back().setFillColor(P1_COLOR);
	else if(player == 2)
	    dot_shapes.back().setFillColor(P2_COLOR);
	
	/* Ustaw odpowiednie wartości w tabeli stanu */
	state.setGridState(player, index);
	
	return index;
   }

   bool checkWin(Vector2u index){
       uint player= state.getGridState(index);
       if(player != 1 && player != 2)
	   return false;

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
       uint total;
       for(int i= xstart; i <= xstop; i++){
	   sindex.x= index.x + i;
	   for(int j= ystart; j <=ystop; j++){
	       sindex.y= index.y + j;
	       /* Jeśli znaleziono sąsiada takiego samego koloru */
	       if(state.getGridState(sindex) == player && (i != 0 || j != 0)){
		   /* Popatrz w jego stronę i w przeciwną i dodaj liczbę kropek*/
		   total= 1 + chase(player, index, Vector2u(i, j));
		   total+= chase(player, index, Vector2u(i*(-1), j*(-1)));
		   
		   if(total >= WIN_CONDITION)
		       return true;
	       }
	   }
       }

       return false;
   }
};

#endif
