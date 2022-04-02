/*
g++ -fno-rtti -std=c++17 main.cpp histogram.a -o main -I$HALIDE_ROOT/include \
    -I$HALIDE_ROOT/tools -L$HALIDE_ROOT/lib -lHalide -lz -ldl \
    -lpthread -ljpeg `libpng-config --cflags --ldflags`
*/

#include <iostream>
#include <fstream>
#include "HalideBuffer.h"
#include "halide_image_io.h"
#include "histogram.h"

using namespace Halide::Runtime;
using namespace Halide::Tools;

int main(int argc, char **argv) {
    if(argc < 2) {
        puts("Usage: ./main path_input");
        return 1;
    }
    const char * path_input = argv[1];
    
    Buffer<uint8_t> input = load_image(path_input);
    Buffer<int32_t> output({256});
    
    histogram(input, output);
    int32_t * data = output.data();
    std::ofstream outFile("results/plotme.dat");
    for (int i = 0; i < 255; i++) {
        outFile << i << " " << * (data + i) << "\n";
        std::cout << * (data + i) << " ";
    }
    outFile.close();
    std::cout << std::endl;

    std::cout << "Success!" << std::endl;
    return 0;
}
