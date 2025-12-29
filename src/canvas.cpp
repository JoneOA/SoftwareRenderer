#include <cmath>
#include <iostream>
#include <math.h>

#include "canvas.h"

Canvas::Canvas(int width, int height, int pitch, unsigned char* pixels)
{
    data.w = width;
    data.h = height;
    data.pitch = pitch;
    data.pixels = pixels;

    m_depth.w = width;
    m_depth.h = height;
    m_depth.pitch = 3;
    m_depth.pixels = new double[width * height * m_depth.pitch]{};
    for(int i = 0; i < width * height * pitch; i++)
        m_depth.pixels[i] = 255.0;
}

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
    double D = 0;

    vec3c colour = {0xff, 0xff, 0xff};

    for(vec2d xy = start; xy.x < end.x; xy.x++){
        if(steep) DrawPoint({xy.y, xy.x}, colour, 3, 1.0);
        else DrawPoint(xy, colour, width, 1.0);
                
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

void Canvas::DrawPoint(vec2d point, vec3c colour, int width, double w)
{
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch)] = colour.x;
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch) + 1] = colour.y;
    data.pixels[((int)point.y * data.pitch * data.w) + ((int)point.x * data.pitch) + 2] = colour.z;
}

//TODO: Change vec2d's into vertex with colour component
//TODO: Fill triangle with colour, will require looping over every pixel in the boudning box to check if it is inside
//      Optimisations can be made
void Canvas::DrawTriangle(vec2d p1, vec2d p2, vec2d p3)
{
    DrawLine(p1, p2, 1.0);
    DrawLine(p2, p3, 1.0);
    DrawLine(p3, p1, 1.0);
}

double TriangleArea (vec2d vec1, vec2d vec2, vec2d vec3)
{
    vec2d A = Vec2Sub(vec2, vec1);
    vec2d B = Vec2Sub(vec3, vec1);

    return Vec2CrossProd(B, A) / 2.0;
}

template <typename T>
bool IsInsideTriange(T vec1, T vec2, T vec3, T point) {
    bool isInside = TriangleArea(vec1, vec2, point) <= 0;
    isInside &= TriangleArea(vec2, vec3, point) <= 0;
    isInside &= TriangleArea(vec3, vec1, point) <= 0;
    return isInside;
}

//TODO: Calculate whether point is inside 3 points to colour solid
void Canvas::GetBarycentricCoords(vec2d tp1, vec2d tp2, vec2d tp3, vec3c c1, vec3c c2, vec3c c3, double depth)
{
    vec2d tl, br;
    tl.x = std::min(std::min(tp1.x, tp2.x), tp3.x);
    tl.y = std::min(std::min(tp1.y, tp2.y), tp3.y);

    br.x = std::max(std::max(tp1.x, tp2.x), tp3.x);
    br.y = std::max(std::max(tp1.y, tp2.y), tp3.y);

    for(int j = int(tl.y); j < br.y; j++)
    {
        bool hasEntered = false;
        for(int i = int(tl.x); i < br.x; i++)
        {
            if(m_depth.pixels[j * m_depth.pitch * m_depth.w + i * m_depth.pitch] > 1.0/depth){
                std::cout << m_depth.pixels[j * m_depth.pitch * m_depth.w + i * m_depth.pitch] << "<" << depth << std::endl;
                continue;
            }
            
            vec2d point = {double(i), double(j)};
            if(IsInsideTriange(tp1, tp2, tp3, point)) {
                DrawPoint(point, c1, 1, 0);
                m_depth.pixels[j * m_depth.pitch * m_depth.w + i * m_depth.pitch] = 1.0/depth;
                m_depth.pixels[j * m_depth.pitch * m_depth.w + i * m_depth.pitch + 1] = 1.0/depth;
                m_depth.pixels[j * m_depth.pitch * m_depth.w + i * m_depth.pitch + 2] = 1.0/depth;
                hasEntered = true;
            }
        }
    }
}

vec2d Canvas::ModelToScreen(vec2d& point)
{
    return {((point.x + 1)/2)*data.w, (1-(point.y + 1)/2)*data.h};
}

void Canvas::DrawTriangle3D(vec3d& p1, vec3d& p2, vec3d& p3, vec3c c1, vec3c c2, vec3c c3, vec3d& n1, vec3d& n2, vec3d& n3)
{

    vec2d tp1 = Vec3Project(p1);
    vec2d tp2 = Vec3Project(p2);
    vec2d tp3 = Vec3Project(p3);

    tp1 = ModelToScreen(tp1);
    tp2 = ModelToScreen(tp2);
    tp3 = ModelToScreen(tp3);


    if(Vec2Winding(tp1, tp2, tp3) <= 0) return;
    vec3d normalVert = Vec3Norm(p1);

    c1.x = (unsigned char)(double(c1.x) * std::max(0.0, fabs(Vec3DotProd(normalVert, n1))));
    c1.y = (unsigned char)(double(c1.y) * std::max(0.0, fabs(Vec3DotProd(normalVert, n1))));
    c1.z = (unsigned char)(double(c1.z) * std::max(0.0, fabs(Vec3DotProd(normalVert, n1))));
    
    vec3d depthVec = {Vec3Mag(p1), Vec3Mag(p2), Vec3Mag(p3)};
    double depth = Vec3Mag(depthVec);

    vec2d tl, br;
    tl.x = std::min(std::min(tp1.x, tp2.x), tp3.x);
    tl.y = std::min(std::min(tp1.y, tp2.y), tp3.y);

    br.x = std::max(std::max(tp1.x, tp2.x), tp3.x);
    br.y = std::max(std::max(tp1.y, tp2.y), tp3.y);

    for(int j = int(tl.y); j < br.y; j++)
    {
        bool hasEntered = false;
        for(int i = int(tl.x); i < br.x; i++)
        {
            if(m_depth.pixels[j * m_depth.pitch * m_depth.w + i * m_depth.pitch] < depth){
                //std::cout << m_depth.pixels[j * m_depth.pitch * m_depth.w + i * m_depth.pitch] << "<" << depth << std::endl;
                continue;
            }
            
            vec2d point = {double(i), double(j)};
            if(IsInsideTriange(tp1, tp2, tp3, point)) {
                DrawPoint(point, c1, 1, 0);
                m_depth.pixels[((int)point.y * m_depth.pitch * data.w) + ((int)point.x * m_depth.pitch)] = depth;
                m_depth.pixels[((int)point.y * m_depth.pitch * data.w) + ((int)point.x * m_depth.pitch) + 1] = depth;
                m_depth.pixels[((int)point.y * m_depth.pitch * data.w) + ((int)point.x * m_depth.pitch) + 2] = depth;
                hasEntered = true;
            } else if (hasEntered)
                break;
        }
    }


    //GetBarycentricCoords(dp1, dp2, dp3, c1, c2, c3, n2.x);
}

void Canvas::ClearDepthBuffer()
{
    for(int i = 0; i < m_depth.w * m_depth.h * m_depth.pitch; i++)
        m_depth.pixels[i] = 255.0;
}


