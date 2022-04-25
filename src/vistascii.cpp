#include "../include/vistascii.h"
#include <fstream>
#include <iostream>

using namespace va;

VertexEntity::VertexEntity(std::string path): entities{} {
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
        entities.push_back(new Trigon(a, b, c)); // (!) leak
    }
    file.close();
}

VertexEntity::~VertexEntity() {
    for (Entity* entity: entities) {
        //delete entity;
    }
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
    int w = _width;
    int h = _height;
    float dx = vexB.x - vexA.x;
    float dy = vexB.y - vexA.y;
    bool steep = abs(dy) > abs(dx);
    if (steep) {
        float tmp = dy;
        dy = dx;
        dx = tmp;
        vexA.swapxy();
        vexB.swapxy();
        h = _width;
        w = _height;
    }
    if (dx < 0) {
        Vec3 tmp = vexA;
        vexA = vexB;
        vexB = tmp;
    }
    for (int x = vexA.x; x <= vexB.x; x++) {
        int y = dy / dx * (x - vexA.x) + vexA.y;
        if (0 <= y && y < h && 0 <= x && x < w) {
            float depth = vexA.z + (vexB.z - vexA.z)*(x - vexA.x)/(vexB.x - vexA.x);
            float stored = steep ? _zbuffer[x][y] : _zbuffer[y][x];
            if (depth < stored || stored == -1) {
                if (steep) {
                    _zbuffer[x][y] = depth;
                } else {
                    _zbuffer[y][x] = depth;
                }
            }
        }
    }
}

void VertexRenderer::render(std::initializer_list<VertexEntity*> el) {
    _zbuffer = -1;
    for (VertexEntity* entity: el) {
        for (Entity* ngon: entity->entities) { 
            for (int i = 0; i < ngon->size(); i++) {
                if (dot(ngon->vertexes()[i], _normal) > 0 || dot(ngon->vertexes()[i + 1 % ngon->size()], _normal) > 0) {
                    drawEdge(persProject(ngon->vertexes()[i], _normal, _focal) + Vec3(_width, _height, 0) / 2,
                        persProject(ngon->vertexes()[(i + 1) % ngon->size()], _normal, _focal) + Vec3(_width, _height, 0) / 2);
                }   
            }
        }
    }
    refresh();
}

void VertexRenderer::refresh() {
    for (int row = 0; row < _zbuffer.height; row++) {
        for (int col = 0; col < _zbuffer.width; col++) {
            mvaddch(row, col, getDepthChar(_zbuffer[row][col]));
        }
    }
    wrefresh(stdscr);
}