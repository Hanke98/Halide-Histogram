/*
g++ -fno-rtti -std=c++17 grayscale.cpp -o grayscale -I$HALIDE_ROOT/include \
    -I$HALIDE_ROOT/tools -L$HALIDE_ROOT/lib -lHalide -lz -ldl \
    -lpthread
*/

#include "Halide.h"
#include <stdio.h>

using namespace Halide;
using namespace Halide::ConciseCasts;

int main(int argc, char **argv) {
    Func grayscale("grayscale");
    Var x("x"), y("y"), c("c");
    Param<int> method;
    ImageParam input(type_of<uint8_t>(), 3);

    Expr r16 = i16(input(x, y, 0));
    Expr g16 = i16(input(x, y, 1));
    Expr b16 = i16(input(x, y, 2));
    
    grayscale(x, y) = mux(method, 
        {u8(0.299f * r16 + 0.587f * g16 + 0.114f * b16), // weighted method
         u8(0.333f * r16 + 0.333f * g16 + 0.333f * b16)} // average method
    );
    
    int vectorSize = get_host_target().natural_vector_size(input.type());
    grayscale.vectorize(x, vectorSize).parallel(y);
    grayscale.compile_to_static_library("grayscale", {input, method}, "grayscale");
    std::cout << "Halide pipeline compiled, but not yet run." << std::endl;

    return 0;
}
