/*
g++ -fno-rtti -std=c++17 main.cpp -o main -I$HALIDE_ROOT/include \
    -I$HALIDE_ROOT/tools -L$HALIDE_ROOT/lib -lHalide -lz -ldl \
    -lpthread -ljpeg `libpng-config --cflags --ldflags`
*/
#include <iostream>
#include "Halide.h"
#include "halide_image_io.h"

using namespace Halide;
using namespace Halide::Tools;
using namespace Halide::ConciseCasts;

int main(int argc, char **argv) {
    Buffer<uint8_t> input = load_image("Germany.png");

    Func brighter("brighter");
    Var x("x"), y("y"), c("c");
    Expr e = f32(input(x, y, c)) * 1.5f;
    brighter(x, y, c) = u8_sat(e);
    brighter.print_loop_nest();

    Buffer<uint8_t> output = 
                brighter.realize({input.width(), input.height(), input.channels()});
    save_image(output, "Germany_brighter.png");
    printf("Success!\n");
    return 0;
}