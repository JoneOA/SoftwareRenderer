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
T Vec2Sub(T vec1, T vec2)
{
    return { vec1.x - vec2.x, vec1.y - vec2.y };
}

