#include "../include/vec3.h"

void Vec3::transform(const std::vector<Vec3>& m) {
    if (m.size() > 3) {
        std::__throw_invalid_argument("Transformation matrix must be 3x3");
    }
    Vec3 res  = *this;
    x = dot(m[0], res);
    y = dot(m[1], res);
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

std::istream& operator>>(std::istream& is, Vec3& v) {
    is >> v.x >> v.y >> v.z;
    return is;
}