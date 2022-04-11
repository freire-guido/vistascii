#include "../include/vistascii.h"
#include <fstream>
#include <iostream>

va::VertexEntity::VertexEntity(std::string path) {
    std::ifstream file(path);
    std::string line;
    while(!file.eof()) {
        float x, y, z;
        file >> x >> y >> z;
        Vec3 a(x, y, z);
        file >> x >> y >> z;
        Vec3 b(x, y, z);
        file >> x >> y >> z;
        Vec3 c(x, y, z);
        trigons.push_back(Trigon(a, b, c));
    }
    file.close();
}

char getDepthChar(int d) {
    std::string charMap = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    if (d < 0 || d >= charMap.size()) {
        d = charMap.size() - 1;
    }
    return charMap[d];
}

//Normalized N
Vec3 deProject(int x, int y, Vec3 N, float dist) {
    float r = N.x * N.x + N.y * N.y + N.z;
    float px = (x*(N.y * N.y + N.z) - y*N.x*N.y) / r;
    float py = (y*(N.x * N.x + N.z) - x*N.x*N.y) / r;
    return {px, py, -(px*N.x+py*N.y) / N.z};
}


// Normalized N
Vec3 persProject(Vec3 v, Vec3 N, float dist) {
    float vN = dot(v, N);
    Vec3 p = v * dist / vN - N;
    v.x = dot(p, {1, 0, -N.x});
    v.y = dot(p, {0, 1, -N.y});
    v.z = vN;
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
    for (const VertexEntity& entity: entities) {
        for (const Trigon& trigon: entity.trigons){ 
            for (int i = 0; i <= trigon.vertexes().size(); i++) {
                if (dot(trigon.vertexes()[i], camera_pos) > 0 || dot(trigon.vertexes()[i + 1 % trigon.vertexes().size()], camera_pos) > 0) {
                    drawEdge(persProject(trigon.vertexes()[i], camera_pos, focal_length) + Vec3(width, height, 0) / 2,
                        persProject(trigon.vertexes()[i + 1 % trigon.vertexes().size()], camera_pos, focal_length) + Vec3(width, height, 0) / 2);
                }   
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