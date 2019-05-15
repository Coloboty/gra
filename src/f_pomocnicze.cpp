
#include <iostream>
#include "f_pomocnicze.h"



/*
RectangleShape linia(Vector2f start, Vector2f end, uint thickness, Color color){
    float distance;
    Vector2f dir= end-start;
    distance= sqrt(dir.x*dir.x + dir.y*dir.y);
    Vector2f unitDir= dir/distance;
    Vector2f unitPerpendicular(-unitDir.y, unitDir.x);
    
    
    RectangleShape rect(Vector2f(distance, thickness*2+1));
    std::cout << distance << '\n';
    
    rect.setOrigin(Vector2f(0, thickness+1));
    rect.setPosition(start);
    
    
    return rect;
}
*/
