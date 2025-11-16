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
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch)] = 0x7F * (width + 1);
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch) + 1] = 0xff;
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch) + 2] = 0x00;
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

//TODO: Calculate whether point is inside 3 points to colour solid
void Canvas::GetBarycentricCoords(vec2d tp1, vec2d tp2, vec2d tp3, vec2d cp1)
{
    
    vec2d BA = Vec2Sub(tp2, tp1);
    vec2d CA = Vec2Sub(tp3, tp1);

    


    vec2d p1, p2;
    p1 = {2, 2};
    p2 = {1, 1};
    vec2d dif = Vec2Sub(p1, p2);

    std::cout << dif.x << ", " << dif.y << std::endl;
    
}
