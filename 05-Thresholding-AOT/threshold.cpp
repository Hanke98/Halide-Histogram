/*
g++ -fno-rtti -std=c++17 threshold.cpp -o threshold -I$HALIDE_ROOT/include \
    -I$HALIDE_ROOT/tools -L$HALIDE_ROOT/lib -lHalide -lz -ldl \
    -lpthread
*/


#include "Halide.h"
#include "constants.hpp"
#include <stdio.h>

using namespace Halide;
using namespace Halide::ConciseCasts;
using namespace Constants;

int main(int argc, char **argv) {
    Func result("result");
    Var x("x"), y("y");
    ImageParam input(type_of<uint8_t>(), 2, "input");
    Param<int> threshold("threshold");
    Param<int> method("method");

    switch (method.get()) {
        case Thresh::BINARY:
            result(x, y) = u8(select(input(x, y) >= threshold, 255, 0)); 
            break;
        case Thresh::BINARY_INV:
            result(x, y) = u8(mux(i16(input(x, y) >= threshold), {255, 0}));
            break;
        case Thresh::TO_ZERO:
            result(x, y) = u8(mux(i16(input(x, y) >= threshold), {0, input(x, y)})); 
            break;
        case Thresh::TO_ZERO_INV:
            result(x, y) = u8(mux(i16(input(x, y) >= threshold), {input(x, y), 0})); 
            break;
        default:
            result(x, y) = u8(mux(i16(input(x, y) >= threshold), {0, 255})); 
            break;
    }

    int vectorSize = get_host_target().natural_vector_size(input.type());
    result.vectorize(x, vectorSize).parallel(y);
    result.compile_to_static_library("threshold", {input, threshold, method}, "threshold");
    std::cout << "Halide pipeline compiled, but not yet run." << std::endl;

    return 0;
}
