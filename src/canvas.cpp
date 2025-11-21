#include <cmath>
#include <iostream>
#include <math.h>

#include "canvas.h"

void Canvas::Draw()
{

}

//Gupta and Sproull algorithm - en.wikipedia.org/wiki/Line_drawing_algorithm#Gupta_and_Sproull_algorithm
//TODO: Take colour component and lerp between the start and end points
void Canvas::DrawLine(vec2d start, vec2d end, int width)
{
    bool steep = abs(end.y - start.y) > abs(end.x - start.x);

    if(steep)
    {
        std::swap(start.x, start.y);
        std::swap(end.x, end.y);
    }
    if(start.x > end.x)
    {
        std::swap(start.x, end.x);
        std::swap(start.y, end.y);
    }

    double dx = end.x - start.x;
    double dy = std::fabs(end.y - start.y);

    int yStep = start.y < end.y ? 1 : -1;

    double d = 2 * dy - dx;// discriminator

    //Used to anti-alias the line
    double length = std::sqrt(dx * dx + dy * dy);
    double s = (double)dx / length; 
    double c = (double)dy / length;
    int D = 0;

    for(vec2d xy = start; xy.x < end.x; xy.x++){
        if(steep) DrawPoint({xy.y, xy.x}, 3);
        else DrawPoint(xy, width);
                
        if(d <= 0)
        {
            D = D + s;
            d = d + 2 * dy;
        }
        else 
        {
            D = D + s - c; 
            d = d + 2 * (dy - dx);
            xy.y+=yStep;
        }
    }
}

void Canvas::DrawPoint(vec2d point, int width)
{
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch)] = 0xB4 * width;
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch) + 1] = 0x69 * width;
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch) + 2] = 0xFF;
}

//TODO: Change vec2d's into vertex with colour component
//TODO: Fill triangle with colour, will require looping over every pixel in the boudning box to check if it is inside
//      Optimisations can be made
void Canvas::DrawTriangle(vec2d p1, vec2d p2, vec2d p3)
{
    DrawLine(p1, p2, 1);
    DrawLine(p2, p3, 1);
    DrawLine(p3, p1, 1);


}

template <typename T>
double TriangleArea (T vec1, T vec2, T vec3)
{
    vec2d A = Vec2Sub(vec2, vec1);
    vec2d B = Vec2Sub(vec3, vec1);

    return Vec2CrossProd(B, A) / 2.0;
}

template <typename T>
bool IsInsideTriange(T vec1, T vec2, T vec3, T point) {
    bool isInside = TriangleArea(vec1, vec2, point) >= 0;
    isInside &= TriangleArea(vec2, vec3, point) >= 0;
    isInside &= TriangleArea(vec3, vec1, point) >= 0;
    return isInside;
}

//TODO: Calculate whether point is inside 3 points to colour solid
void Canvas::GetBarycentricCoords(vec2d tp1, vec2d tp2, vec2d tp3, vec2d cp1)
{
    double triangleArea = TriangleArea(tp1, tp2, tp3);

    vec2d tl, br;
    tl.x = std::min(std::min(tp1.x, tp2.x), tp3.x);
    tl.y = std::min(std::min(tp1.y, tp2.y), tp3.y);

    br.x = std::max(std::max(tp1.x, tp2.x), tp3.x);
    br.y = std::max(std::max(tp1.y, tp2.y), tp3.y);


    for(int j = tl.y; j < br.y; j++)
    {
        bool hasEntered = false;
        for(int i = tl.x; i < br.x; i++)
        {
            double area = 0;
            vec2d point = {i, j};

            if(IsInsideTriange(tp1, tp2, tp3, point)) {
                DrawPoint(point, 1);
                hasEntered = true;
            } else if(hasEntered)
                break;
            else
                DrawPoint(point, 0);

           
        }
    }
}
   
