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
        std::vector<VertexEntity> entities;
        Vec3 camera_pos;
        VertexRenderer(std::initializer_list<VertexEntity> el, Vec3 cp = {0, 0, 0}): entities{el}, camera_pos{cp} {};
    };
}
