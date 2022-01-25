#include "include/vistascii.h"

int main() {
    va::VertexRenderer ver({va::VertexEntity("./data/tank")});
    Vec3 move;
    while (true) {
        int key = getch();
        if (key == 'w') {
            move = {0, 0, 0.1};
        } else if (key == 's') {
            move  = {0, 0, -0.1};
        } else if (key == 'd') {
            move = {0.1, 0, 0};
        } else if (key == 'a') {
            move = {-0.1, 0, 0};
        }
        for (va::VertexEntity& ventity: ver.entities) {
            ventity.move(move);
        }
        ver.render();
    }
    return 0;
}