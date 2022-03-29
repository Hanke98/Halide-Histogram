#include <iostream>
#include "Halide.h"

using namespace Halide;

int main(int argc, char **argv) {
    Func gradient("gradient");
    Var x("x"), y("y");

    Expr e = x + y;
    gradient(x, y) = e;
    gradient.print_loop_nest();

    Buffer<int32_t> output = gradient.realize({800, 600});

    for (int j = 0; j < output.height(); j++) {
        for (int i = 0; i < output.width(); i++) {
            if (output(i, j) != i + j) {
                printf("Something went wrong!\n"
                       "Pixel %d, %d was supposed to be %d, but instead it's %d\n",
                       i, j, i + j, output(i, j));
                return -1;
            }
        }
    }

    printf("Success!\n");
    return 0;
}