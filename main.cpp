#include "include/vistascii.h"

int main() {
    va::VertexRenderer ver({});
    for (float i = -30; i < 30; i+=6) {
        ver.add({{i, 0, 1}, {i + 1, 1, 1}, {i + 2, 0, 1}});
        ver.add({{i + 3, 1, 1}, {i + 4, 0, 1}, {i + 5, 1, 1}});
    }
    std::vector<Vec3> tMatrix;
    while (true) {
        int key = getch();
        if (key == 'q') {
            tMatrix = {{1,0,0}, {0,cos(0.01),-sin(0.01)}, {0,sin(0.01),cos(0.01)}};
        } else if (key == 'e') {
            tMatrix = {{1,0,0}, {0,cos(-0.01),-sin(-0.01)}, {0,sin(-0.01),cos(-0.01)}};
        }
        for (va::VertexEntity& ventity: ver.entities) {
            for (Vec3& vertex: ventity.vertexes) {
                vertex.transform(tMatrix);
            }
        }
        ver.render();
        refresh();
    }
    endwin();
    return 0;
}