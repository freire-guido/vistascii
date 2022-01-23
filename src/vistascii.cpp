#include "../include/vistascii.h"

char getDepthChar(int d) {
    std::string charMap = ".,-~:;=!*#$@";
    if (d < 0 || d / 2 >= charMap.size()) {
        d = charMap.size() * 2 - 1;
    }
    return charMap[d / 2];
}

void va::VertexRenderer::render() {
    std::vector<std::string> window(height, std::string(width, getDepthChar(-1)));
    for (VertexEntity& entity: entities) {
        for (Vec3& vertex: entity.vertexes) {
            Vec3 projection = vertex / dot(vertex, camera_pos);
            window[projection.y][projection.x] = getDepthChar(vertex.length());
        }
    }
    for (int row = 0; row < window.size(); row++) {
        mvprintw(row, 0, window[row].c_str());
    }
    refresh();
}