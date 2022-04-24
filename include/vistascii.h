#pragma once

#include <string>
#include <vector>
#include <ncurses.h>
#include "vec3.h"

namespace va {
    struct Entity {
        virtual int size() const = 0;
        virtual Vec3* vertexes() = 0;
        virtual void move(const Vec3&);
        virtual void transform(const std::vector<Vec3>&);
    };
    struct Trigon: Entity {
        Vec3 _vertexes[3];
        Trigon(Vec3 a, Vec3 b, Vec3 c): _vertexes{a, b, c} {};
        int size() const {
            return 3;
        }
        Vec3* vertexes() {
            return _vertexes;
        }
        void move(const Vec3& direction) {
            _vertexes[0] += direction;
            _vertexes[1] += direction;
            _vertexes[2] += direction;
        }
        void transform(const std::vector<Vec3>& m) {
            _vertexes[0].transform(m);
            _vertexes[1].transform(m);
            _vertexes[2].transform(m);
        }
    };
    struct VertexEntity {
        std::vector<Entity> entities;
        VertexEntity(std::initializer_list<Entity> pl): entities{pl} {};
        VertexEntity(std::string path);
        void move(const Vec3& direction) {
            for (Entity& entity: entities) {
                entity.move(direction);
            }
        }
        void transform(const std::vector<Vec3>& m) {
            for (Entity& entity: entities) {
                entity.transform(m);
            }
        }
    };

    class VertexRenderer {
    public:
        VertexRenderer(std::initializer_list<VertexEntity> el, Vec3 cp = {0, 0, 1}, float fl = 10): _entities{el}, _normal{cp}, _focal{fl} {
            initscr();
            noecho();
            getmaxyx(stdscr, _height, _width);
            _zbuffer = DepthWindow(_height, _width);
        }
        ~VertexRenderer() {
            endwin();
        };
        void add(const VertexEntity& ve) {
            _entities.push_back(ve);
        }
        std::vector<VertexEntity>& entities() {
            return _entities;
        };
        void drawEdge(Vec3 vexA, Vec3 vexB);
        void render();
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
        std::vector<VertexEntity> _entities;
        DepthWindow _zbuffer;
        Vec3 _normal;
        float _focal;
    };
}