#pragma once
#include <stdint.h>
#include "vector.h"

class Canvas
{
public:
    struct data {
        int w;
        int h;
        int pitch;
        unsigned char* pixels;
    } data;

    double angle = 1;
    
    void Draw();
    void DrawLine(vec2d start, vec2d end, int width);
    void DrawPoint(vec2d point, int size);
    void DrawTriangle(vec2d p1, vec2d p2, vec2d p3);
    void GetBarycentricCoords(vec2d tp1, vec2d tp2, vec2d tp3, vec2d cp1);

};
