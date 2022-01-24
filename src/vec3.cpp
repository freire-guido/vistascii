#include "../include/vec3.h"

void Vec3::transform(const std::vector<Vec3>& m) {
    Vec3 res  = *this;
    x = m[0].x*res.x + m[0].y*res.y + m[0].z*res.z;
    y = m[1].x*res.x + m[1].y*res.y + m[1].z*res.z; // Should be dot products
    z = m[2].x*res.x + m[2].y*res.y + m[2].z*res.z;
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