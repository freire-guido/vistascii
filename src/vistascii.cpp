#include "../include/vistascii.h"

char getDepthChar(int d) {
    std::string charMap = "@$#*!;:=~-,.";
    if (d < 0 || d / 2 >= charMap.size()) {
        d = charMap.size() * 2 - 1;
    }
    return charMap[d / 2];
}

Vec3 persproject(Vec3 v, Vec3 n, float d) {
    Vec3 p = v * d / dot(v, n) - normalize(n);
    v.x = dot(p, {1, 0, -n.x});
    v.y = dot(p, {0, 1, -n.y});
    return v;
}

void va::VertexRenderer::render() {
    std::vector<std::string> window(height, std::string(width, getDepthChar(-1)));
    for (VertexEntity& entity: entities) {
        for (Vec3& vertex: entity.vertexes) {
            if (dot(vertex, camera_pos) > 0) {
                Vec3 projection = persproject(vertex, camera_pos, 1) + Vec3(width, height, 0) / 2;
                if (0 <= projection.x && projection.x < width && 0 <= projection.y && projection.y < height) {
                    window[projection.y][projection.x] = getDepthChar(vertex.length());
                }
            }   
        }
    }
    for (int row = 0; row < window.size(); row++) {
        mvprintw(row, 0, window[row].c_str());
    }
    refresh();
}