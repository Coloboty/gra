#include "plansza.h"

board::board(const Vector2f &gridSize, const Vector2f &screenSize){
    grid_size= gridSize;
    screen_size= screenSize;
    state= new uint[((int)grid_size.x-1) * ((int)grid_size.y-1)];
    grid_color= Color(150, 150, 150);
    board_color= Color(220, 220, 220);
    grid_thickness= 2;
    
    /* Policz odległość między granicami kratek */
    grid_spacing= Vector2f(screen_size.x/grid_size.x, screen_size.y/grid_size.y);
    Vector2f pos= getPosition();

    /* Rysuj kratki (bez brzegów) */
    for(uint i= 1; i < grid_size.x; i++){
	grid.push_back(LineShape(Vector2f(grid_spacing.x*i+pos.x, pos.y), Vector2f(grid_spacing.x*i+pos.x, screen_size.y+pos.y), grid_thickness, grid_color));
    }

    for(uint i= 1; i < grid_size.y; i++){
	grid.push_back(LineShape(Vector2f(pos.x, grid_spacing.y*i+pos.y), Vector2f(screen_size.x+pos.x, grid_spacing.y*i+pos.y), grid_thickness, grid_color));
    }

    /* Rysuj brzegi osobno, trochę grubsze */
    uint border_thickness= grid_thickness*2+5;
    Color border_color(75, 75, 75);

    grid.push_back(LineShape(Vector2f(pos.x, pos.y), Vector2f(pos.x, screen_size.y+pos.y), border_thickness, border_color));
    grid.push_back(LineShape(Vector2f(screen_size.x+pos.x, pos.y), Vector2f(screen_size.x+pos.x, screen_size.y+pos.y), border_thickness, border_color));
    grid.push_back(LineShape(Vector2f(pos.x, pos.y), Vector2f(screen_size.x+pos.x, pos.y), border_thickness, border_color));
    grid.push_back(LineShape(Vector2f(pos.x, screen_size.y+pos.y), Vector2f(screen_size.x+pos.x, screen_size.y+pos.y), border_thickness, border_color));
}

/* Bierze pozycję na ekranie i zwraca wektor najbliższego przecięcia linii */
Vector2f board::snapToGrid(Vector2i coords){
    Vector2f result;
    Vector2f mod;

    /* Weź indeks przecięcia */
    result.x= floor(coords.x/grid_spacing.x);
    result.y= floor(coords.y/grid_spacing.y);

    /* Weź resztę */
    mod.x= coords.x%(int)grid_spacing.x;
    mod.y= coords.y%(int)grid_spacing.y;

    /* Sprawdź, do którego przecięcia jest bliżej i przesuń się tam w razie potrzeby */
    result.x*=grid_spacing.x;
    if(mod.x > grid_spacing.x/2)
	result.x+= grid_spacing.x;
	
    result.y*=grid_spacing.y;
    if(mod.y > grid_spacing.y/2)
	result.y+= grid_spacing.y;

    /* Sprawdź, czy nie jesteś na krawędzi */
    if(result.x > screen_size.x-grid_spacing.x)
	result.x= screen_size.x-grid_spacing.x;

    if(result.y > screen_size.y-grid_spacing.y)
	result.y= screen_size.y-grid_spacing.y;

    if(result.x < grid_spacing.x)
	result.x= grid_spacing.x;

    if(result.y < grid_spacing.y)
	result.y= grid_spacing.y;
	
    return result;
}
