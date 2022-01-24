#include "include/vistascii.h"

int main() {
    va::VertexEntity vex = {{-4, 1, 2}, {0, -2, 4}, {4, 1, 2}}; 
    va::VertexRenderer ver({vex});
    Vec3 move;
    while (true) {
        ver.render();
        refresh();
        for (va::VertexEntity& ventity: ver.entities) {
            for (Vec3& vertex: ventity.vertexes) {
                vertex = vertex + move; 
            }
        }
        int key = getch();
        if (key == 'a') {
            move = {0, 0, 0.1};
        } else if (key == 'b') {
            move = {-0 , -0, -0.1};
        }
    }
    endwin();
    return 0;
}