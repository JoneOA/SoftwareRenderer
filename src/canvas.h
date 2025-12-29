#pragma once
#include <stdint.h>
#include "vector.h"

class Canvas
{
public:
    struct Data {
        int w;
        int h;
        int pitch;
        unsigned char* pixels;
    } data;

    struct Depth {
        int w;
        int h;
        int pitch;
        double* pixels;
    } m_depth;   

    double angle = 1;
    
    Canvas(int width, int height, int pitch, unsigned char* pixels);
    void Draw();
    void DrawLine(vec2d start, vec2d end, int width);
    void DrawPoint(vec2d point, vec3c colour, int size, double w);
    void DrawTriangle(vec2d p1, vec2d p2, vec2d p3);
    void DrawTriangle3D(vec3d& p1, vec3d& p2, vec3d& p3, vec3c c1, vec3c c2, vec3c c3, vec3d& n1, vec3d& n2, vec3d& n3);
    void GetBarycentricCoords(vec2d tp1, vec2d tp2, vec2d tp3, vec3c c1, vec3c c2, vec3c c3, double depth);
    void ClearDepthBuffer();

    vec2d ModelToScreen(vec2d& point);
};

