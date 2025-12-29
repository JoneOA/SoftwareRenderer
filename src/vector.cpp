#include "vector.h"

vec3d Vec3CrossProd(vec3d vec1, vec3d vec2)
{
    vec3d vec3;

    vec3.x = (vec1.y * vec2.z) - (vec2.y * vec1.z);
    vec3.y = (vec1.z * vec2.x) - (vec2.z * vec1.x);
    vec3.z = (vec1.x * vec2.y) - (vec2.x * vec1.y);

    return vec3;
}

double Vec3DotProd(vec3d vec1, vec3d vec2)
{
    return (vec1.x * vec2.x) + (vec1.y + vec2.y) + (vec1.z + vec2.z);
}
