/*
g++ -fno-rtti -std=c++17 main.cpp threshold.a grayscale.a -o main -I$HALIDE_ROOT/include \
    -I$HALIDE_ROOT/tools -L$HALIDE_ROOT/lib -lHalide -lz -ldl \
    -lpthread -ljpeg `libpng-config --cflags --ldflags`
*/

#include <iostream>
#include "HalideBuffer.h"
#include "halide_image_io.h"
#include "threshold.h"
#include "grayscale.h"
#include "constants.hpp"

using namespace Halide::Runtime;
using namespace Halide::Tools;
using namespace Constants;

std::string get_output_name(std::string out_name, int method);

int main(int argc, char **argv) {
    /* Parsing input */
    if (argc < 5) {
        std::cout << "Usage: ./main path_input out_name threshold_value use_all method";
        std::cout << "\nMethods available: (use the code)";
        for (int i = 0; i < 4; i++) {
            std::cout << "\n" << get_method_name(i) << " | Code: " << i << "\n";
        }
        return 1;
    }
    const char * path_input = argv[1];
    const std::string out_name = std::string(argv[2]);
    const int threshold_value = (int) atof(argv[3]);;
    const bool useAll = std::string(argv[4]) == "true";
    int method;
    if (!useAll) {
        if (argc < 6) {
            std::cout << "Usage: ./main path_input out_name threshold_value use_all method";
            std::cout << "\nIf use-all is false, specify the method";
            std::cout << "\nMethods available: (use the code)";
            for (int i = 0; i < 4; i++) {
                std::cout << "\n" << get_method_name(i) << " | Code: " << i << "\n";
            }
            return 1;
        }
        method = (int) atof(argv[5]);
    }
    /* End parsing input */

    Buffer<uint8_t> input = load_image(path_input);
    Buffer<uint8_t> inputGray({input.width(), input.height()});
    grayscale(input, 0, inputGray);
    
    if (!useAll) {
        Buffer<uint8_t> output({input.width(), input.height()});
        threshold(inputGray, threshold_value, method, output);
        save_image(output, get_output_name(out_name, method));
    } else {
        Buffer<uint8_t> outputs[4];
        for (int i = 0; i < 4; i++) {
            outputs[i] = Buffer<uint8_t> ({input.width(), input.height()});
            threshold(inputGray, threshold_value, i, outputs[i]);
            save_image(outputs[i], get_output_name(out_name, i));   
        }
    }

    std::cout << "Success!" << std::endl;
    return 0;
}

std::string get_output_name(std::string out_name, int method) {
    std::string prefix = "results/", ext = ".png";
    return prefix + out_name + get_method_name(method) + ext;
}