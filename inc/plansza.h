#ifndef PLANSZA_H
#define PLANSZA_H

#include <SFML/Graphics.hpp>
#include "f_pomocnicze.h"

using namespace std;
using namespace sf;

class board : public Drawable, public Transformable{
    Vector2f grid_size, screen_size;
    uint *state;
    vector<LineShape> grid;
    Vector2f grid_spacing;
    Color grid_color;
    Color board_color;
    uint grid_thickness;

    virtual void draw(RenderTarget &target, RenderStates states) const{
	RectangleShape background(screen_size);
	background.setPosition(getPosition());
	target.draw(background, states);
	for(uint i= 0; i < grid.size(); i++)
	    target.draw(grid[i], states);
    }
    
public:
    board(const Vector2f &gridSize, const Vector2f &screenSize);

    ~board(){
	delete state;
    }

    Vector2f snapToGrid(Vector2i coords);

    /* Bierze pozycję przecięcia ekranu i zwraca indeks w macierzy */
    Vector2i getGridIndex(Vector2f coords){
	Vector2i result;
	result.x= coords.x/grid_spacing.x - 1;
	result.y= coords.y/grid_spacing.y - 1;

	return result;
    }
};

#endif
