#include "include/vistascii.h"

int main() {
    va::VertexRenderer ver({});
    for (float x = -30; x < 30; x+=6) {
        for (float z = 2; z < 10; z++) {
            int r = rand();
            if (0 <= r && r < 715827882) {
                ver.add({{x, 0, z}, {x + 1, 1, z}, {x + 2, 0, z}});
            } else if (r <= 715827882 && r < 1431655764) {
                ver.add({{x + 3, 1, z}, {x + 4, 0, z}, {x + 5, 1, z}});
            }
        }
    }
    std::vector<Vec3> tMatrix = {{1,0,0}, {0,1,0}, {0,0,1}};
    while (true) {
        int key = getch();
        for (va::VertexEntity& ventity: ver.entities) {
            for (Vec3& vertex: ventity.vertexes) {
                vertex = vertex + Vec3(0, 0, -0.01);
            }
        }
        ver.render();
        refresh();
    }
    return 0;
}