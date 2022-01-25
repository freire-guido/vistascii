#pragma once

#include <string>
#include <vector>
#include <ncurses.h>
#include "vec3.h"

namespace va {
    struct VertexEntity {
        std::vector<Vec3> vertexes;
        VertexEntity(std::initializer_list<Vec3> vl): vertexes{vl} {};
        void add(Vec3 v) {
            vertexes.push_back(v);
        }
        void remove(int i) {
            vertexes.erase(vertexes.begin() + i);
        }
    };
    struct VertexRenderer {
        int height, width;
        std::vector<VertexEntity> entities;
        std::vector<std::vector<float>> depthWindow;
        Vec3 camera_pos;
        VertexRenderer(std::initializer_list<VertexEntity> el, Vec3 cp = {0, 0, 1}): entities{el}, camera_pos{cp} {
            initscr();
            noecho();
            getmaxyx(stdscr, height, width);
        }
        ~VertexRenderer() {
            endwin();
        };
        void add(VertexEntity ve) {
            entities.push_back(ve);
        }
        void drawEdge(const Vec3& vexA, const Vec3& vexB);
        void render();
    };
}