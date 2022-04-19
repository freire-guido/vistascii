#pragma once

#include <vector>
#include <math.h>

struct Vec3 {
    float x, y, z;
    Vec3(): x{0}, y{0}, z{0} {};
    Vec3(float _x, float _y, float _z): x{_x}, y{_y}, z{_z} {};
    Vec3 operator+(Vec3 v) {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    void operator+=(Vec3 v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    Vec3 operator-(Vec3 v) {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    void operator-=(Vec3 v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    Vec3 operator*(Vec3 v) {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    void operator*=(Vec3 v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }
    Vec3 operator*(float f) {
        return Vec3(x*f, y*f, z*f);
    }
    Vec3 operator/(Vec3 v) {
        return Vec3(x / v.x, y / v.y, z / v.z);
    }
    void operator/=(Vec3 v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
    }
    Vec3 operator/(float f) {
        return Vec3(x / f, y / f, z / f);
    }
    void operator/=(float f) {
        x /= f;
        y /= f;
        z /= f;
    }
    float length() {
        return sqrt(x*x + y*y + z*z);
    }
    void swapxy() {
        float tmp = x;
        x = y;
        y = tmp;
    };
    void transform(const std::vector<Vec3>& m);
};

Vec3 normalize(Vec3 v);

Vec3 cross(Vec3 a, Vec3 b);

float dot(Vec3 a, Vec3 b);