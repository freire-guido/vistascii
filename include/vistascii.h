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
    // 2d float array resource handle
    struct DepthWindow {
        int height, width;
        float** window;
        DepthWindow() : height{0}, width{0}, window{nullptr} {}
        DepthWindow(int h, int w, float f = -1) : height{h}, width{w} {
            window = new float*[height];
            for (int i = 0; i < height; i++) {
                window[i] = new float[width];
                for (int j = 0; j < width; j++) {
                    window[i][j] = f;
                }
            }
        }
        ~DepthWindow() {
            for (int i = 0; i < height; i++) {
                delete[] window[i];
            }
            delete[] window;
        }
        DepthWindow(const DepthWindow& dw): height{dw.height}, width{dw.width}, window{new float*[dw.height]} {
            for (int i = 0; i < height; i++) {
                window[i] = new float[width];
                for (int j = 0; j < width; j++) {
                    window[i][j] = dw.window[i][j];
                }
            }
        }
        DepthWindow& operator=(const DepthWindow& dw) {
            for (int i = 0; i < height; i++) {
                delete[] window[i];
            }
            delete[] window;
            height = dw.height;
            width = dw.width;
            window = new float*[height];
            for (int i = 0; i < height; i++) {
                window[i] = new float[width];
                for (int j = 0; j < width; j++) {
                    window[i][j] = dw.window[i][j];
                }
            }
            return *this;
        }
        DepthWindow& operator=(float f) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    window[i][j] = f;
                }
            }
            return *this;
        }
        float* operator[](int i) {
            return window[i];
        }
    };
    struct VertexRenderer {
        int height, width;
        std::vector<VertexEntity> entities;
        DepthWindow z_buffer;
        Vec3 camera_pos;
        float focal_length;
        VertexRenderer(std::initializer_list<VertexEntity> el, Vec3 cp = {0, 0, 1}, float fl = 10): entities{el}, camera_pos{cp}, focal_length{fl} {
            initscr();
            noecho();
            getmaxyx(stdscr, height, width);
            z_buffer = DepthWindow(height, width);
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