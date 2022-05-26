#include "../include/vistascii.h"
#include <fstream>
#include <iostream>

using namespace va;

Ngon::Ngon(std::initializer_list<Vec3> vxs): size{vxs.size()}, vertexes{new Vec3[vxs.size()]} {
    auto it = vxs.begin();
    for (int i = 0; i < size; i++) {
        vertexes[i] = *it++;
    }
}

Ngon::Ngon(std::string path, int s): size{s}, vertexes{new Vec3[s]} {
    std::ifstream file(path);
    std::string line;
    for (int i = 0; i < s; i++) {
        float x, y, z;
        file >> vertexes[i];
    }
    file.close();
}

VertexEntity::VertexEntity(std::string path) {
    std::ifstream file(path);
    while(!file.eof()) {
        unsigned int size;
        file >> size;
        Ngon ngon(size);
        for (int i = 0; i < size; i++) {
            file >> ngon[i];
        }
        ngons.push_back(ngon);
    }
    file.close();
}

char VertexRenderer::getDepthChar(int d) {
    std::string charMap = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    d = 100 / abs(_focal) * d;
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
    Vec3 p = v * dist / vN - N*dist;
    v.x = dot(p, {1, 0, -N.x});
    v.y = dot(p, {0, 1, -N.y});
    v.z = vN;
    return v;
}

void VertexRenderer::moveCamera(const Vec3& m) {
    _normal = normalize(_normal + m);
}

void VertexRenderer::binEdge(Vec3 vexA, Vec3 vexB, ScanLine yBins[]) {
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
            float z = vexA.z + (vexB.z - vexA.z)*(x - vexA.x)/(vexB.x - vexA.x);
            if (steep) {
                if (y > yBins[x].xStart) {
                    if (y > yBins[x].xEnd) {
                        yBins[x].xStart = yBins[x].xEnd;
                        yBins[x].zStart = yBins[x].zEnd;
                    }
                    yBins[x].xEnd = y;
                    yBins[x].zEnd = z;
                } else {
                    yBins[x].xEnd = yBins[x].xStart;
                    yBins[x].zEnd = yBins[x].zStart;
                    yBins[x].xStart = y;
                    yBins[x].zStart = z;
                }
            } else {
                if (x > yBins[y].xStart) {
                    if (x > yBins[y].xEnd) {
                        yBins[y].xStart = yBins[y].xEnd;
                        yBins[y].zStart = yBins[y].zEnd;
                    }
                    yBins[y].xEnd = x;
                    yBins[y].zEnd = z;
                } else {
                    yBins[y].xEnd = yBins[y].xStart;
                    yBins[y].zEnd = yBins[y].zStart;
                    yBins[y].xStart = x;
                    yBins[y].zStart = z;
                }
            }
        }
    }
}

void VertexRenderer::renderEntity(VertexEntity entity) {
    for (Ngon& ngon: entity.ngons) { 
        ngon[0] = persProject(ngon[0], _normal, _focal) + Vec3(_width, _height, 0);
        int maxY, minY = ngon[0].y;
        for (int i = 1; i < ngon.size; i++) {
            ngon[i] = persProject(ngon[i], _normal, _focal) + Vec3(_width, _height, 0);
            if (ngon[i].y > maxY) {
                maxY = ngon[i].y;
            } else if (ngon[i].y < minY) {
                minY = ngon[i].y;
            }
        }
        ScanLine yBins[maxY - minY];
        for (int i = 0; i < ngon.size; i++) {
            binEdge(ngon[i], ngon[(i + 1) % ngon.size], yBins);
        }
        for (int i = 0; i < maxY - minY; i++) {
            for (int x = yBins[i].xStart; x < yBins[i].xEnd; x++) {
                _zbuffer[i + minY][x] = yBins[i].zStart + (yBins[i].zEnd - yBins[i].zStart)*(x - yBins[i].xStart)/(yBins[i].xEnd - yBins[i].xStart);
            }
        }

    }
}

void VertexRenderer::refresh() {
    for (int row = 0; row < _zbuffer.height; row++) {
        for (int col = 0; col < _zbuffer.width; col++) {
            mvaddch(row, col, getDepthChar(_zbuffer[row][col]));
        }
    }
    wrefresh(stdscr);
}