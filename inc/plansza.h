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
	Vector2u index;
	uint player;
    dot(Vector2u _index, uint _player): exists(true), index(_index), player(_player) {}
    dot(): exists(false) {}
    };
    
    dot *grid;
    vector<dot*> *neighbours;
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

    void setGridState(dot state, Vector2u index) {grid[index.x + (uint)grid_size.y*index.y]= state;}
    dot &getGridState(Vector2u index) {return grid[index.x + (uint)grid_size.y*index.y];}
    void addNeighbour(dot *neighbour, Vector2u index) {neighbours[index.x + (uint)grid_size.y*index.y].push_back(neighbour);}
    vector<dot*> &getNeighbourList(Vector2u index) {return neighbours[index.x + (uint)grid_size.y*index.y];}
    
public:
    board(const Vector2u &gridSize, const Vector2f &screenSize);
    ~board(){
	delete[] grid;
	delete[] neighbours;
    }

    Vector2f snapToGrid(Vector2i coords);
    Vector2u getGridIndex(Vector2f coords);
    Vector2f getGridCoords(Vector2u index);
    bool doesExist(Vector2u index) {return getGridState(index).exists;}
    /* uint getDotId(Vector2u index) {return getGridState(index).id;} */
    /* uint getDotGroup(Vector2u index) {return graph.find(getGridState(index).id);} */
    Vector2u getDotIndex(Vector2u index) {return getGridState(index).index;}
    void *getDotAddress(Vector2u index) {return &getGridState(index);}
    
    bool isValidMove(uint player, Vector2u index){
	if(index.x > grid_size.x || index.y > grid_size.y)
	    return false;

	if(getGridState(index).exists == true){
	    return false;
	}

	return true;
    }

    void listNeighbours(Vector2u index){
	vector<dot*> list= getNeighbourList(index);

	cout << "Sąsiedzi:" << endl;
	for(uint i= 0; i < list.size(); i++)
	    cout << "Sąsiad " << i << "- X: " << list[i]->index.x << "  Y: " << list[i]->index.y << endl;
    }
    
    bool addDot(uint player, Vector2u index){
	/* Sprawdź, czy ruch jest dozwolony */
	if(!isValidMove(player, index))
	   return false;

	/* Dodaj graficzną reprezentację do listy rzeczy do wyświetlania */
	Vector2f coords= getGridCoords(index);
	dot_shapes.push_back(CircleShape(DOT_RADIUS));
	dot_shapes.back().setFillColor(Color::Red);
	dot_shapes.back().setPosition(coords.x-DOT_RADIUS, coords.y-DOT_RADIUS);
	
	/* Ustaw odpowiednie wartości w tabeli stanu */
	setGridState(dot(index, player), index);
	cout << "Dodano kropkę" << endl;

	/* Przygotowanie do szukania sąsiadów */
	int xstart, xstop, ystart, ystop;
	if(index.x > 0) xstart= -1;
	else xstart= 0;
	
	if(index.x < grid_size.x) xstop= 1;
	else xstop= 0;

	if(index.y > 0) ystart= -1;
	else ystart= 0;

	if(index.y < grid_size.y) ystop= 1;
	else ystop= 0;

	/* Szukanie sąsiadów w obrębie jednej kratki */
	dot *search_dot;
	Vector2u search_index;
	bool neighbour_flag= false;
	for(int i= xstart; i <= xstop; i++){
	    for(int j= ystart; j <= ystop; j++){
		search_index= Vector2u(index.x+i, index.y+j);
		search_dot= &getGridState(search_index);
		
		if(search_dot->exists){
		    if(!(i == 0 && j == 0)){
			neighbour_flag= true;
			addNeighbour(search_dot, index);
			addNeighbour(&getGridState(index), search_index);
			/* cout << "Znaleziono sąsiada: " << search_dot << endl; */
		    }
		}
	    }
	}

	/* Jeśli znaleziono sąsiada przeprowadź detekcję cyklu */
	if(neighbour_flag){
	    /* depthSearch(index); */
	}
	
	return true;
    }

    bool depthSearch(Vector2u index){
	dot *start= &getGridState(index);
	vector<dot*> neighbours;
	dot *current, *previous;
	bool *visited;
	bool away= false;

	/* Inicjalizacja */
	
	visited= new bool[grid_size.x*grid_size.y];
	for(uint i= 0; i < grid_size.x*grid_size.y; i++){
	    visited[i]= false;
	}

	
	stack<dot*> unvisited;
	unvisited.push(start);
	previous= start;
	
	cout << "Początek przeszukiwania" << endl;

	/* Póki są jeszcze miejsca nieodwiedzone */
	while(!unvisited.empty()){
	    /* Zdejmij punkt ze stosu */
	    current= unvisited.top();
	    unvisited.pop();
	    /* Zaznacz jako odwiedzony */
	    visited[current->index.x + current->index.y*grid_size.y]= true;
	    cout << "Odwiedzam ";
	    cout << "X: " << current->index.x << "  Y: " << current->index.y << endl;

	    /* Znajdź wszystkich jego sąsiadów */
	    neighbours= getNeighbourList(current->index);
	    for(uint i= 0; i < neighbours.size(); i++){
		/* Jeżeli sąsiad jeszcze nie był odwiedzony */
		if(!visited[neighbours[i]->index.x + neighbours[i]->index.y*grid_size.y]){
		    /* Wepchnij nieodwiedzonego sąsiada na stos punktów do odwiedzenia */
		    unvisited.push(neighbours[i]);
		}
		/* Jeśli sąsiad już był odwiedzony */
		else{
		    /* Jeśli nie jest to poprzednio odwiedzony punkt */
		    if(neighbours[i] != previous){
			/* Jeżeli sąsiadem jest punkt początkowy */
			if(neighbours[i] == start){
			    cout << "Znaleziono cykl!" << endl;
			    return true;
			}
		    }
		}
	    } /* KONIEC PĘTLI FOR */
	    
	    previous= current;
	}
	
	delete[] visited;
	return false;
    }
};



#endif
