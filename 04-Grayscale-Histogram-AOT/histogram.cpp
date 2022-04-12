/*
g++ -fno-rtti -std=c++17 histogram.cpp -o histogram -I$HALIDE_ROOT/include \
    -I$HALIDE_ROOT/tools -L$HALIDE_ROOT/lib -lHalide -lz -ldl \
    -lpthread
*/

#include "Halide.h"
#include <stdio.h>

using namespace Halide;
using namespace Halide::ConciseCasts;

int main(int argc, char **argv) {
    Func histogram("histogram");
    Var x("x"), y("y"), i("i"), ii("ii"), io("io");

    ImageParam input(type_of<uint8_t>(), 2);

    histogram(i) = 0;
    
    RDom r(input);
    histogram(input(r.x, r.y)) += 1;
    histogram.compute_root();
    histogram.compile_to_static_library("include/histogram", {input}, "histogram");
    std::system("mv include/*.a lib/");
    std::cout << "Halide pipeline compiled, but not yet run." << std::endl;

    return 0;
}
