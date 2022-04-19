#include "../include/vistascii.h"
#include <fstream>
#include <iostream>

using namespace va;

VertexEntity::VertexEntity(std::string path) {
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
    return Vec3(px, py, -(px*N.x+py*N.y) / N.z) + N*dist;
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

void VertexRenderer::drawEdge(Vec3 vexA, Vec3 vexB) {
    float dx = vexB.x - vexA.x;
    float dy = vexB.y - vexA.y;
    bool steep = abs(dy) > abs(dx);
    if (steep) {
        float tmp = dy;
        dy = dx;
        dx = tmp;
        vexA.swapxy();
        vexB.swapxy();
    }
    if (dx < 0) {
        Vec3 tmp = vexA;
        vexA = vexB;
        vexB = tmp;
    }
    for (int x = vexA.x; x <= vexB.x; x++){
        int y = dy / dx * (x - vexA.x) + vexA.y;
        if (0 <= x && x < width && 0 <= y && y < height) {
            float depth = vexA.z + (vexB.z - vexA.z)*(x - vexA.x)/(vexB.x - vexA.x);
            float stored = steep ? z_buffer[x][y] : z_buffer[y][x];
            if (depth < stored || stored == -1) {
                if (steep) {
                    z_buffer[x][y] = depth;
                } else {
                    z_buffer[y][x] = depth;
                }
            }
        }
    }
}

void VertexRenderer::render() {
    z_buffer = -1;
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
    for (int row = 0; row < z_buffer.height; row++) {
        for (int col = 0; col < z_buffer.width; col++) {
            mvaddch(row, col, getDepthChar(z_buffer[row][col]));
        }
    }
    refresh();
}