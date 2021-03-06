#ifndef F_POMOCNICZE_H
#define F_POMOCNICZE_H

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

/* FUNKCJE -------------------------------------------------------- */

/* Liczy odległość między dwoma punktami */
float vDistance(Vector2f v1, Vector2f v2);

/* KLASY ---------------------------------------------------------- */

class LineShape : public Drawable{
public:
    LineShape(const Vector2f &point1, const Vector2f &point2, uint thick, const Color &col)
	{
	    color= col;
	    thickness= thick;
	    Vector2f direction = point2 - point1;
	    Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
	    Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

	    Vector2f offset = (thickness/2.f)*unitPerpendicular;


	    vertices[0].position = point1 + offset;
	    vertices[1].position = point2 + offset;
	    vertices[2].position = point2 - offset;
	    vertices[3].position = point1 - offset;

	    for (int i=0; i<4; ++i)
		vertices[i].color = col;
	}
    



private:
    Vertex vertices[4];
    float thickness;
    Color color;

    virtual void draw(RenderTarget &target, RenderStates states) const{
	target.draw(vertices,4,Quads, states);
    }
};
/* RectangleShape linia(Vector2f start, Vector2f end, uint thickness, Color color); */

#endif
