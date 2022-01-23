#include "../include/vec3.h"

Vec3 normalize(Vec3 v) {
    return v / v.length();
}

Vec3 cross(Vec3 a, Vec3 b) {
    return Vec3(a.y - b.z, a.z - b.x, a.x - b.y);
}

float dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}