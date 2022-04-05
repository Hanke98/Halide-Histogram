/*
g++ -fno-rtti -std=c++17 threshold.cpp -o threshold -I$HALIDE_ROOT/include \
    -I$HALIDE_ROOT/tools -Iinclude -L$HALIDE_ROOT/lib -lHalide -lz -ldl \
    -lpthread
*/

#include <iostream>
#include <fstream>
#include "Halide.h"
#include "constants.hpp"

using namespace Halide;
using namespace Halide::ConciseCasts;
using namespace Constants;

int main(int argc, char **argv) {
    Func threshold_binary("threshold_binary");
    Func threshold_binary_inv("threshold_binary_inv");
    Func threshold_to_zero("threshold_to_zero");
    Func threshold_to_zero_inv("threshold_to_zero_inv");
    Var x("x"), y("y");
    ImageParam input(type_of<uint8_t>(), 2, "input");
    Param<int> threshold("threshold");

    threshold_binary(x, y) = u8(select(input(x, y) >= threshold, 255, 0)); 
    threshold_binary_inv(x, y) = u8(select(input(x, y) >= threshold, 0, 255));
    threshold_to_zero(x, y) = u8(select(input(x, y) >= threshold, input(x, y), 0)); 
    threshold_to_zero_inv(x, y) = u8(select(input(x, y) >= threshold, 0, input(x, y))); 

    int vectorSize = get_host_target().natural_vector_size(input.type());

    threshold_binary.vectorize(x, vectorSize).parallel(y);
    threshold_binary_inv.vectorize(x, vectorSize).parallel(y);
    threshold_to_zero.vectorize(x, vectorSize).parallel(y);
    threshold_to_zero_inv.vectorize(x, vectorSize).parallel(y);
    
    Func functions[4] = {threshold_binary, threshold_binary_inv, 
                         threshold_to_zero, threshold_to_zero_inv};
    
    std::ofstream header("include/threshold.h");
    for (Func fn : functions) {
        string name = fn.name();
        fn.compile_to_static_library("include/" + name, {input, threshold}, name);
        header << "#include \"" + name + ".h\"" << std::endl;
    }
    header.close();

    std::system("mv include/*.a lib/");
    std::cout << "Halide pipeline compiled, but not yet run." << std::endl;

    return 0;
}
