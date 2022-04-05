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
    if(argc < 3) {
        puts("Usage: ./main path_input_image path_output_image");
        return 1;
    }
    const char * path_input = argv[1];
    const char * path_output = argv[2];
    Buffer<uint8_t> input = load_image(path_input);

    Func brighter("brighter");
    Var x("x"), y("y"), c("c");
    Expr e = f32(input(x, y, c)) * 1.5f;
    brighter(x, y, c) = u8_sat(e);
    brighter.reorder(c, x, y);
    brighter.print_loop_nest();

    Buffer<uint8_t> output = 
                brighter.realize({input.width(), input.height(), input.channels()});
    save_image(output, path_output);
    printf("Success!\n");
    return 0;
}