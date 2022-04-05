#include <iostream>

using namespace std;

namespace Constants {
    enum Thresh {
        BINARY,
        BINARY_INV,
        TO_ZERO,
        TO_ZERO_INV
    };

    string get_method_name(int method) {
        string methods[] = {"binary", "binary_inv", "to_zero", "to_zero_inv"};
        return methods[method];
    }
}
