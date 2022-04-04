#pragma once

#include <string>
#include <vector>
#include <ncurses.h>
#include "vec3.h"

namespace va {
    struct Trigon {
        Vec3 a, b, c;
        Trigon();
        Trigon(Vec3 _a, Vec3 _b, Vec3 _c): a{_a}, b{_b}, c{_c} {};
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
        std::vector<Trigon> trigons;
        VertexEntity(std::initializer_list<Trigon> pl): trigons{pl} {};
        VertexEntity(std::string path);
        void move(Vec3 direction) {
            for (Trigon& trigon: trigons) {
                trigon.move(direction);
            }
        }
        void transform(const std::vector<Vec3>& m) {
            for (Trigon& trigon: trigons) {
                trigon.transform(m);
            }
        }
    };
    struct VertexRenderer {
        int height, width;
        std::vector<VertexEntity> entities;
        std::vector<std::vector<float>> depthWindow;
        Vec3 camera_pos;
        float focal_length;
        VertexRenderer(std::initializer_list<VertexEntity> el, Vec3 cp = {0, 0, 1}, float fl = 10): entities{el}, camera_pos{cp}, focal_length{fl} {
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