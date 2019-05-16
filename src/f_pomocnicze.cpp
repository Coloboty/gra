
#include <iostream>
#include "f_pomocnicze.h"

float vDistance(Vector2f v1, Vector2f v2){
    float x, y;
    x= v1.x-v2.x;
    y= v1.y-v2.y;
    return sqrt(x*x + y*y);
}
