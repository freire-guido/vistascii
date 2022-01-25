#include "../include/vec3.h"

void Vec3::transform(const std::vector<Vec3>& m) {
    Vec3 res  = *this;
    x = dot(m[0], res);
    y = dot(m[1], res); // Should be dot products
    z = dot(m[2], res);
}

Vec3 normalize(Vec3 v) {
    return v / v.length();
}

Vec3 cross(Vec3 a, Vec3 b) {
    return Vec3(a.y - b.z, a.z - b.x, a.x - b.y);
}

float dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}