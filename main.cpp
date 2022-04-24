#include "include/vistascii.h"
#include <iostream>

using namespace va;
int main() {
    va::VertexRenderer ver({va::VertexEntity("./data/tank")}, {0,0,1}, 0.5);
    ver.entities()[0].move({0, 10, 1});
    Vec3 move;
    while (true) {
        ver.render();
        int key = getch();
        if (key == 'w') {
            move = {0, 0.5, 0};
        } else if (key == 's') {
            move  = {0, -0.5, 0};
        } else if (key == 'd') {
            move = {0.5, 0, 0};
        } else if (key == 'a') {
            move = {-0.5, 0, 0};
        } else if (key == 'r') {
            move = {0, 0, 0.5};
        } else if (key == 'f') {
            move = {0, 0, -0.5};
        }
        ver.entities()[0].move(move);
    }
    return 0;
}