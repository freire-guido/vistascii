#include "../include/vistascii.h"

char getDepthChar(int d) {
    std::string charMap = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    if (d < 0 || d / 2 >= charMap.size()) {
        d = charMap.size() * 2 - 1;
    }
    return charMap[d / 2];
}

Vec3 persproject(Vec3 v, Vec3 n, float dist) {
    Vec3 p = v * dist / dot(v, n) - normalize(n);
    v.x = dot(p, {1, 0, -n.x});
    v.y = dot(p, {0, 1, -n.y});
    v.z = dot(v, normalize(n));
    return v;
}

void va::VertexRenderer::drawEdge(const Vec3& vexA, const Vec3& vexB) {
    float dy = vexB.y - vexA.y;
    float dx = vexB.x - vexA.x;
    for (int x = vexA.x; x <= vexB.x; x++){
        int y = dy / dx * (x - vexA.x) + vexA.y;
        if (0 <= x && x < width && 0 <= y && y < height) {
            // Z buffer
            float depth = vexA.z + (vexB.z - vexA.z)*(x - vexA.x)/(vexB.x - vexA.x);
            if (depth < depthWindow[y][x] || depthWindow[y][x] == -1) {
                depthWindow[y][x] = depth;
            }
        }
    }
}

void va::VertexRenderer::render() {
    depthWindow = std::vector(height, std::vector<float>(width, -1));
    for (VertexEntity& entity: entities) {
        for (int i = 1; i <= entity.vertexes.size(); i++) {
            if (dot(entity.vertexes[i-1], camera_pos) > 0 || dot(entity.vertexes[i % entity.vertexes.size()], camera_pos) > 0) {
                Vec3 projectionA = persproject(entity.vertexes[i - 1], camera_pos, 1) + Vec3(width, height, 0) / 2;
                Vec3 projectionB = persproject(entity.vertexes[i % entity.vertexes.size()], camera_pos, 1) + Vec3(width, height, 0) / 2;
                if (entity.vertexes[i - 1].x > entity.vertexes[i % entity.vertexes.size()].x) {
                    Vec3 tmp = projectionA;
                    projectionA = projectionB; // Swap projection vectors
                    projectionB = tmp;
                }
                drawEdge(projectionA, projectionB);
            }   
        }
    }
    for (int row = 0; row < depthWindow.size(); row++) {
        for (int col = 0; col < depthWindow[row].size(); col++) {
            mvaddch(row, col, getDepthChar(depthWindow[row][col]));
        }
    }
    refresh();
}