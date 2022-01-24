#include "include/vistascii.h"

int main() {
    va::VertexEntity vex = {{-12, 0, 10}, {12, 0, 2}}; 
    va::VertexRenderer ver({vex});
    ver.render();
    getch();
    endwin();
    return 0;
}