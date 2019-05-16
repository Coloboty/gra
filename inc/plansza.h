#ifndef PLANSZA_H
#define PLANSZA_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"
#include "f_pomocnicze.h"
#include "union_find.hpp"

using namespace std;
using namespace sf;

class board : public Drawable, public Transformable{
    Vector2f screen_size;
    Vector2u grid_size;
    vector<LineShape> grid_lines;
    Vector2f grid_spacing;
    Color grid_color;
    Color board_color;
    uint grid_thickness;

    struct dot{
	bool exists;
	uint id;
	uint player;
    dot(uint _id, uint _player): exists(true), id(_id), player(_player) {}
    dot(): exists(false), id(0), player(0) {}
    };
    
    dot *grid;
    vector<CircleShape> dot_shapes;
    union_find<uint> graph;

    virtual void draw(RenderTarget &target, RenderStates states) const{
	RectangleShape background(screen_size);
	background.setPosition(getPosition());
	
	target.draw(background, states);
	
	for(uint i= 0; i < grid_lines.size(); i++)
	    target.draw(grid_lines[i], states);

	for(uint i= 0; i < dot_shapes.size(); i++)
	    target.draw(dot_shapes[i], states);
    }

    void setGridState(dot state, Vector2u coords) {grid[coords.x + (uint)grid_size.y*coords.y]= state;}
    dot getGridState(Vector2u coords) {return grid[coords.x + (uint)grid_size.y*coords.y];}
    
public:
    board(const Vector2u &gridSize, const Vector2f &screenSize);

    ~board(){
	delete grid;
    }

    Vector2f snapToGrid(Vector2i coords);

    /* Bierze pozycję przecięcia ekranu i zwraca indeks w macierzy */
    Vector2u getGridIndex(Vector2f coords){
	Vector2u result;
	result.x= coords.x/grid_spacing.x - 1;
	result.y= coords.y/grid_spacing.y - 1;
	return result;
    }

    Vector2f getGridCoords(Vector2u index){
	Vector2f result;
	result.x= index.x*grid_spacing.x + grid_spacing.x;
	result.y= index.y*grid_spacing.y + grid_spacing.y;
	return result;
    }

    bool isValidMove(uint player, Vector2u index){
	if(index.x > grid_size.x || index.y > grid_size.y)
	    return false;

	if(getGridState(index).exists == true){
	    return false;
	}

	return true;
    }

    bool doesExist(Vector2u index) {return getGridState(index).exists;}
    uint getDotId(Vector2u index) {return getGridState(index).id;}
    uint getDotGroup(Vector2u index) {return graph.find(getGridState(index).id);}
    
    bool addDot(uint player, Vector2u index){
	Vector2f coords;
	dot new_dot, search_dot;
	if(!isValidMove(player, index))
	   return false;

	coords= getGridCoords(index);

	dot_shapes.push_back(CircleShape(DOT_RADIUS));
	dot_shapes.back().setFillColor(Color::Red);
	dot_shapes.back().setPosition(coords.x-DOT_RADIUS, coords.y-DOT_RADIUS);
	new_dot.exists= true;
	new_dot.player= player;
	new_dot.id= graph.makeSet(0);
	
	setGridState(new_dot, index);

	int xstart, xstop, ystart, ystop;
	if(index.x > 0) xstart= -1;
	else xstart= 0;
	
	if(index.x < grid_size.x) xstop= 1;
	else xstop= 0;

	if(index.y > 0) ystart= -1;
	else ystart= 0;

	if(index.y < grid_size.y) ystop= 1;
	else ystop= 0;
	    
	for(int i= xstart; i <= xstop; i++){
	    for(int j= ystart; j <= ystop; j++){
		search_dot= getGridState(Vector2u(index.x+i, index.y+j));
		/* cout << index.x+i << "   " << index.y+j << endl; */
		if(search_dot.exists){
		    if(graph.find(new_dot.id) ==  graph.find(search_dot.id))
			if(!(i == 0 && j == 0))
			    cout << "Cycle detected" << endl;
		    graph.join(search_dot.id, new_dot.id);
		}
	    }
	}

	/* cout <<  << endl; */
	
	return true;
    }
};

#endif
