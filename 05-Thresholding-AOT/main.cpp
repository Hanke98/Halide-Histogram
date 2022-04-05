/*
g++ -fno-rtti -std=c++17 main.cpp lib/grayscale.a \
    lib/threshold_binary.a lib/threshold_binary_inv.a \
    lib/threshold_to_zero.a lib/threshold_to_zero_inv.a \
    -o main -I$HALIDE_ROOT/include -I$HALIDE_ROOT/tools \
    -Iinclude -L$HALIDE_ROOT/lib -Llib -lHalide -lz -ldl \
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
void print_methods();

int main(int argc, char **argv) {
    /* Parsing input */
    if (argc < 5) {
        std::cout << "Usage: ./main path_input out_name threshold_value use_all method";
        print_methods();
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
            print_methods();
            return 1;
        }
        method = (int) atof(argv[5]);
    }
    /* End parsing input */

    Buffer<uint8_t> input_ = load_image(path_input);
    Buffer<uint8_t> input({input_.width(), input_.height()});
    grayscale(input_, 0, input);
    
    if (!useAll) {
        std::cout << "Using: " << get_method_name(method) << std::endl;
        Buffer<uint8_t> output({input.width(), input.height()});
        switch (method)
        {
        case Thresh::BINARY:
            threshold_binary(input, threshold_value, output);
            break;

        case Thresh::BINARY_INV:
            threshold_binary(input, threshold_value, output);
            break;

        case Thresh::TO_ZERO:
            threshold_binary(input, threshold_value, output);
            break;
        
        case Thresh::TO_ZERO_INV:
            threshold_binary(input, threshold_value, output);
            break;
        
        default:
            std::cout << "(" << method << ") is not a valid method!" << std::endl;
            print_methods();
            return 1;
        }
        save_image(output, get_output_name(out_name, method));
    } else {
        Buffer<uint8_t> output({input.width(), input.height()});

        {
            threshold_binary(input, threshold_value, output);
            save_image(output, get_output_name(out_name, Thresh::BINARY));
        }

        {
            threshold_binary_inv(input, threshold_value, output);
            save_image(output, get_output_name(out_name, Thresh::BINARY_INV));
        }

        {
            threshold_to_zero(input, threshold_value, output);
            save_image(output, get_output_name(out_name, Thresh::TO_ZERO));
        }

        {
            threshold_to_zero_inv(input, threshold_value, output);
            save_image(output, get_output_name(out_name, Thresh::TO_ZERO_INV));
        }
    }

    std::cout << "Success!" << std::endl;
    return 0;
}

std::string get_output_name(std::string out_name, int method) {
    std::string prefix = "results/", ext = ".png";
    return prefix + out_name + "_" + get_method_name(method) + ext;
}

void print_methods() {
    std::cout << "\nMethods available: (use the code)";
    for (int i = 0; i < 4; i++) {
        std::cout << "\n" << get_method_name(i) << " | Code: " << i << "\n";
    }
}