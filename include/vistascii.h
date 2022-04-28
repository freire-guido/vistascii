#pragma once

#include <string>
#include <vector>
#include <ncurses.h>
#include "vec3.h"

namespace va {
    struct Ngon {
        const unsigned int size;
        Vec3* vertexes;
        Ngon(unsigned int s): size{s}, vertexes{new Vec3[s]} {};
        Ngon(std::initializer_list<Vec3> vxs): size{vxs.size()}, vertexes{new Vec3[vxs.size()]} {
            auto it = vxs.begin();
            for (int i = 0; i < size; i++) {
                vertexes[i] = *it++;
            }
        };
        Ngon(const Ngon& n): size{n.size} {
            vertexes = new Vec3[size];
            for (int i = 0; i < size; i++) {
                vertexes[i] = n[i];
            }
        }
        const Vec3& operator[](int i) const {
            return vertexes[i];
        }

        virtual void move(const Vec3& direction) {
            for (int i = 0; i < size; i++) {
                vertexes[i] += direction;
            }
        }
        virtual void transform(const std::vector<Vec3>& m) {
            for (int i = 0; i < size; i++) {
                vertexes[i].transform(m);
            }
        }
        virtual ~Ngon() {
            delete[] vertexes;
        };
    };
    struct VertexEntity {
        std::vector<Ngon> ngons;
        VertexEntity(std::initializer_list<Ngon> ns): ngons{ns} {};
        VertexEntity(std::string path);
        void move(const Vec3& direction) {
            for (Ngon& ngon: ngons) {
                ngon.move(direction);
            }
        }
        void transform(const std::vector<Vec3>& m) {
            for (Ngon& ngon: ngons) {
                ngon.transform(m);
            }
        }
    };

    class VertexRenderer {
    public:
        VertexRenderer(Vec3 cp = {0, 0, 1}, float fl = 10): _normal{cp}, _focal{fl} {
            initscr();
            noecho();
            getmaxyx(stdscr, _height, _width);
            _zbuffer = DepthWindow(_height, _width);
        }
        ~VertexRenderer() {
            endwin();
        };
        void drawEdge(Vec3 vexA, Vec3 vexB);
        void render(const std::vector<VertexEntity>& el);
        void refresh();

    private:
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
        int _height, _width;
        DepthWindow _zbuffer;
        Vec3 _normal;
        float _focal;
    };
}