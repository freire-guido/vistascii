#include "include/vistascii.h"

int main() {
    va::VertexRenderer ver({va::VertexEntity("./data/tank")});
    ver.entities[0].move({0,0,3});
    Vec3 move;
    while (true) {
        ver.render();
        int key = getch();
        if (key == 'w') {
            move = {0, 0.1, 0};
        } else if (key == 's') {
            move  = {0, -0.1, 0};
        } else if (key == 'd') {
            move = {0.1, 0, 0};
        } else if (key == 'a') {
            move = {-0.1, 0, 0};
        }
        ver.camera_pos += move;
        ver.camera_pos /= ver.camera_pos.length();
    }
    return 0;
}