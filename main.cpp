#include "include/vistascii.h"

int main() {
    Vec3 v = {0, 1, 2};
    va::VertexEntity vex = {v};
    vex.add(Vec3(0, 3, 2));
    cross(v, Vec3(1,0,0));
    return 0;
}