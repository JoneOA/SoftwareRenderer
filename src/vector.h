#pragma once
#include <iostream>

struct vec2i {
    int x;
    int y;
};

struct vec2d {
    double x;
    double y;

    inline vec2d operator-(){return {-x, -y};};
    inline vec2d operator-(const vec2d& a){return {a.x - x, a.y - y};};
    inline vec2d operator+(const vec2d& a){return {a.x + x, a.y + y};};
    friend std::ostream& operator<<(std::ostream& a, vec2d b)
    {
        a << "(" << b.x << "," << b.y << ")";
        return a;
    }
};

struct vec3i {
    int x;
    int y;
    int z;
};

struct vec3d {
    double x;
    double y;
    double z;
};

struct vec3c {
    unsigned char x;
    unsigned char y;
    unsigned char z;
};

template <typename T>
T Vec2Sub(const T& vec1, const T& vec2)
{
    return { vec1.x - vec2.x, vec1.y - vec2.y };
}

template <typename T>
double Vec2CrossProd(const T& vec1, const T& vec2)
{
    return (vec1.x * vec2.y) - (vec1.y * vec2.x);
}

template <typename T>
T Vec2CrossProdV(const T& vec1, const T& vec2)
{
    return {-(vec2.y - vec1.y), vec2.x - vec1.x };
}

template <typename T>
double Vec2DotProd(const T& vec1, const T& vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

template <typename T>
double Vec3DotProd(const T& vec1, const T& vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}

template <typename T>
double Vec3Mag(const T& vec)
{
    return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

template <typename T>
T Vec3Norm(const T& vec)
{
    double mag = Vec3Mag(vec); 

    return {vec.x/mag, vec.y/mag, vec.z/mag};
}

template <typename T>
T Vec3CrossProd(const T& vec1, const T& vec2)
{
    T cross = { vec1.y * vec2.z - vec1.z * vec2.y,
                vec1.z * vec2.x - vec1.x * vec2.z,
                vec1.x * vec2.y - vec1.y * vec2.x};
    return cross;
}

template <typename T>
T Vec3Sum(const T& vec1, const T& vec2)
{
    return {vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z};
}

template <typename T>
T Vec3Scale(const T& vec1, const T& vec2)
{
    return {vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z};
}

template <typename T>
vec2d Vec3Project(const T& vec)
{
    return {vec.x / vec.z, vec.y / vec.z};
} 

template <typename T>
double Vec2Winding(const T& vec1, const T& vec2, const T& vec3)
{
    T u = Vec2Sub(vec1, vec2);
    T v = Vec2Sub(vec1, vec3);
    
    return Vec2CrossProd(u, v);
}

template <typename T>
T Lerp(const T& a, const T& b, const T& ratio)
{
    return a + ratio * (b - a);
}
