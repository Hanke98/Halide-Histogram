/*
g++ -fno-rtti -std=c++17 main.cpp grayscale.a -o main -I$HALIDE_ROOT/include \
    -I$HALIDE_ROOT/tools -L$HALIDE_ROOT/lib -lHalide -lz -ldl \
    -lpthread -ljpeg `libpng-config --cflags --ldflags`
*/

#include <iostream>
#include "HalideBuffer.h"
#include "halide_image_io.h"
#include "grayscale.h"

using namespace Halide::Runtime;
using namespace Halide::Tools;

int main(int argc, char **argv) {
    if(argc < 3) {
        puts("Usage: ./main path_input method path_output");
        return 1;
    }
    const char * path_input = argv[1];
    const int method = (int) atof(argv[2]);
    const char * path_output = argv[3];

    Buffer<uint8_t> input = load_image(path_input);
    Buffer<uint8_t> output({input.width(), input.height()});

    grayscale(input, method, output);
    Halide::Tools::save_image(output, path_output);

    std::cout << "Success!" << std::endl;
    return 0;
}
