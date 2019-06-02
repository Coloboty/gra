#include "plansza.h"

board::board(){
    grid_size.x= BOARD_SIZE_X-1;
    grid_size.y= BOARD_SIZE_Y-1;
    screen_size= Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    grid_color= Color(150, 150, 150);
    board_color= Color(220, 220, 220);
    grid_thickness= 2;

    /* for(uint i=0; i < grids; i++) */
	/* grid[i]= 0; */
    
    /* Policz odległość między granicami kratek */
    grid_spacing= Vector2f(screen_size.x/BOARD_SIZE_X, screen_size.y/BOARD_SIZE_Y);
    Vector2f pos= getPosition();

    /* Rysuj kratki (bez brzegów) */
    for(uint i= 1; i < BOARD_SIZE_X; i++){
	grid_lines.push_back(LineShape(Vector2f(grid_spacing.x*i+pos.x, pos.y), Vector2f(grid_spacing.x*i+pos.x, screen_size.y+pos.y), grid_thickness, grid_color));
    }

    for(uint i= 1; i < BOARD_SIZE_Y; i++){
	grid_lines.push_back(LineShape(Vector2f(pos.x, grid_spacing.y*i+pos.y), Vector2f(screen_size.x+pos.x, grid_spacing.y*i+pos.y), grid_thickness, grid_color));
    }

    /* Rysuj brzegi osobno, trochę grubsze */
    uint border_thickness= grid_thickness*2;
    Color border_color(75, 75, 75);

    grid_lines.push_back(LineShape(Vector2f(pos.x, pos.y), Vector2f(pos.x, screen_size.y+pos.y), border_thickness, border_color));
    grid_lines.push_back(LineShape(Vector2f(screen_size.x+pos.x, pos.y), Vector2f(screen_size.x+pos.x, screen_size.y+pos.y), border_thickness, border_color));
    grid_lines.push_back(LineShape(Vector2f(pos.x, pos.y), Vector2f(screen_size.x+pos.x, pos.y), border_thickness, border_color));
    grid_lines.push_back(LineShape(Vector2f(pos.x, screen_size.y+pos.y), Vector2f(screen_size.x+pos.x, screen_size.y+pos.y), border_thickness, border_color));
}

Vector2f board::getGridCoords(Vector2u index){
    Vector2f result;
    result.x= index.x*grid_spacing.x + grid_spacing.x/2;
    result.y= index.y*grid_spacing.y + grid_spacing.y/2;
    return result;
}

Vector2u board::addDot(uint player, uint row){
    Vector2u index= state.addDot(player, row);
	
    /* Dodaj graficzną reprezentację do listy rzeczy do wyświetlania */
    Vector2f coords= getGridCoords(index);
    dot_shapes.push_back(CircleShape(DOT_RADIUS));
    dot_shapes.back().setPosition(coords.x-DOT_RADIUS, coords.y-DOT_RADIUS);
    if(player == 1)
	dot_shapes.back().setFillColor(P1_COLOR);
    else if(player == 2)
	dot_shapes.back().setFillColor(P2_COLOR);
       	
    return index;
}
