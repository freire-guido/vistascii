#pragma once

#include <string>
#include <vector>
#include <ncurses.h>
#include "vec3.h"

namespace va {
    struct Polygon {
        Vec3 a, b, c;
        Polygon();
        Polygon(Vec3 _a, Vec3 _b, Vec3 _c): a{_a}, b{_b}, c{_c} {};
        std::vector<Vec3> vertexes() const {
            return {a, b, c};
        }
        void move(Vec3 direction) {
            a += direction;
            b += direction;
            c += direction;
        }
        void transform(const std::vector<Vec3>& m) {
            a.transform(m);
            b.transform(m);
            c.transform(m);
        }
    };
    struct VertexEntity {
        std::vector<Polygon> polygons;
        VertexEntity(std::initializer_list<Polygon> pl): polygons{pl} {};
        VertexEntity(std::string path);
        void move(Vec3 direction) {
            for (Polygon& polygon: polygons) {
                polygon.move(direction);
            }
        }
        void transform(const std::vector<Vec3>& m) {
            for (Polygon& polygon: polygons) {
                polygon.transform(m);
            }
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