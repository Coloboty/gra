#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "f_pomocnicze.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

#define BOARD_SIZE_X 10
#define BOARD_SIZE_Y 10

using namespace sf;
using namespace std;

enum player{BLUE, RED};

float vDistance(Vector2f v1, Vector2f v2){
    float x, y;
    x= v1.x-v2.x;
    y= v1.y-v2.y;
    return sqrt(x*x + y*y);
}

class board : public Drawable, public Transformable{
    Vector2f grid_size, screen_size;
    uint *state;
    vector<LineShape> grid;
    Vector2f grid_spacing;
    Color grid_color;
    Color board_color;
    const uint grid_thickness= 2;

    virtual void draw(RenderTarget &target, RenderStates states) const{
	/* target.draw(vertices,4,Quads, states); */
	RectangleShape background(screen_size);
	background.setPosition(getPosition());
	target.draw(background, states);
	for(uint i= 0; i < grid.size(); i++)
	    target.draw(grid[i], states);
    }
    
public:
    board(const Vector2f &gridSize, const Vector2f &screenSize){
	grid_size= gridSize;
	screen_size= screenSize;
	state= new uint[((int)grid_size.x-1) * ((int)grid_size.y-1)];
	grid_color= Color(150, 150, 150);
	board_color= Color(220, 220, 220);
	/* setPosition(10, 10); */

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
	
	/* cout << grid_spacing.x << "  " << grid_spacing.y << '\n'; */
    }

    ~board(){
	delete state;
    }

    Vector2f snapToGrid(Vector2i coords){
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

    Vector2i getGridIndex(Vector2f coords){
	Vector2i result;
	result.x= coords.x/grid_spacing.x - 1;
	result.y= coords.y/grid_spacing.y - 1;

	return result;
    }
};

int main()
{
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "wadowice");
    board plansza(Vector2f(BOARD_SIZE_X, BOARD_SIZE_Y), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    Font czcionka;
    Text tekst, snap, indeks;
    CircleShape kropka(11);
    Vector2i mysz, ind;
    Vector2f smysz;
    
    if(!czcionka.loadFromFile("assets/LinLibertine.ttf")){
	cout << "NIE WCZYTANO CZCIONKI" << endl;
	return 1;
    }

    tekst.setFont(czcionka);
    tekst.setCharacterSize(24);
    tekst.setFillColor(Color::Black);
    tekst.setPosition(20, 10);

    snap.setFont(czcionka);
    snap.setCharacterSize(24);
    snap.setString(to_string(2137));
    snap.setFillColor(Color::Black);
    snap.setPosition(20, 50);

    indeks.setFont(czcionka);
    indeks.setCharacterSize(24);
    indeks.setString(to_string(2137));
    indeks.setFillColor(Color::Black);
    indeks.setPosition(20, 90);

    /* kropka.setOrigin(5,5); */
    kropka.setFillColor(Color(200, 150, 150));
        
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

	    if(event.type == Event::KeyReleased)
		if(event.key.code == Keyboard::Escape)
		    window.close();
        }

	mysz= Mouse::getPosition(window);
	smysz= plansza.snapToGrid(mysz);
	ind= plansza.getGridIndex(smysz);
	tekst.setString("X: " + to_string(mysz.x) + "   Y: " + to_string(mysz.y));
	snap.setString("X: " + to_string(smysz.x) + "   Y: " + to_string(smysz.y));
	indeks.setString("X: " + to_string(ind.x) + "   Y: " + to_string(ind.y));

	kropka.setPosition(smysz.x-kropka.getRadius(), smysz.y-kropka.getRadius());
		
        window.clear();
	
	window.draw(plansza);
	window.draw(tekst);
	window.draw(snap);
	window.draw(indeks);
	window.draw(kropka);
	
        window.display();
    }

    return 0;
}
