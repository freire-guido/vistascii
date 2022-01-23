#include "include/vistascii.h"

int main() {
    va::VertexEntity vex = {{1, 0, 2}, {10, 0, 2}}; 
    va::VertexRenderer ver({vex});
    ver.render();
    getch();
    endwin();
    return 0;
}