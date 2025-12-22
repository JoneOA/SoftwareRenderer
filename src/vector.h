#pragma once

struct vec2i {
    int x;
    int y;
};

struct vec2d {
    double x;
    double y;
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

template <typename T>
T Vec2Sub(T& vec1, T& vec2)
{
    return { vec1.x - vec2.x, vec1.y - vec2.y };
}

template <typename T>
double Vec2CrossProd(T& vec1, T& vec2)
{
    return (vec1.x * vec2.y) - (vec1.y * vec2.x);
}

template <typename T>
T Vec2CrossProdV(T& vec1, T& vec2)
{
    return {-(vec2.y - vec1.y), vec2.x - vec1.x };
}

template <typename T>
double Vec2DotProd(T& vec1, T& vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

template <typename T>
double Vec3DotProd(T& vec1, T& vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}

template <typename T>
T Vec3Norm(T& vec)
{
    double mag = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z)); 

    return {vec.x/mag, vec.y/mag, vec.z/mag};
}

template <typename T>
T Vec3CrossProd(T& vec1, T& vec2)
{
    T cross = { vec1.y * vec2.z - vec1.z * vec2.y,
                vec1.z * vec2.x - vec1.x * vec2.z,
                vec1.x * vec2.y - vec1.y * vec2.x};
    return cross;
}

template <typename T>
double Vec2Winding(T& vec1, T& vec2, T& vec3)
{
    T u = Vec2Sub(vec1, vec2);
    T v = Vec2Sub(vec1, vec3);
    
    return Vec2CrossProd(u, v);
}

