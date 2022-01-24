#include "../include/vistascii.h"

char getDepthChar(int d) {
    std::string charMap = "@$#*!;:=~-,. ";
    if (d < 0 || d / 2 >= charMap.size()) {
        d = charMap.size() * 2 - 1;
    }
    return charMap[d / 2];
}

Vec3 persproject(Vec3 v, Vec3 n, float d) {
    Vec3 p = v * d / dot(v, n) - normalize(n);
    v.x = dot(p, {1, 0, -n.x});
    v.y = dot(p, {0, 1, -n.y});
    v.z = dot(v, normalize(n));
    return v;
}

void va::VertexRenderer::render() {
    std::vector<std::string> window(height, std::string(width, getDepthChar(-1)));
    for (VertexEntity& entity: entities) {
        for (int i = 1; i <= entity.vertexes.size(); i++) {
            if (dot(entity.vertexes[i-1], camera_pos) > 0 || dot(entity.vertexes[i % entity.vertexes.size()], camera_pos) > 0) {
                Vec3 projectionA = persproject(entity.vertexes[i - 1], camera_pos, 1) + Vec3(width, height, 0) / 2;
                Vec3 projectionB = persproject(entity.vertexes[i % entity.vertexes.size()], camera_pos, 1) + Vec3(width, height, 0) / 2;
                float dy = projectionB.y - projectionA.y;
                float dx = projectionB.x - projectionA.x;
                for (int x = projectionA.x; x <= projectionB.x; x++){
                    int y = dy / dx * (x - projectionA.x) + projectionA.y;
                    if (0 <= x && x < width && 0 <= y && y < height) {
                        window[y][x] = getDepthChar(projectionA.z + (projectionB.z - projectionA.z)*(x - projectionA.x)/(projectionB.x - projectionA.x));
                    }
                }
            }   
        }
    }
    for (int row = 0; row < window.size(); row++) {
        mvprintw(row, 0, window[row].c_str());
    }
    refresh();
}