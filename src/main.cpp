#include <string>
#include <iostream>

#include "gap_instance.h"

int main(int argc, char** argv) {
    std::string filename = "instances/gap/gap_a/a05100";
    std::cout << "Reading file " << filename << std::endl;

    // Aca empieza la magia 🪄

    GapInstance instance(filename);

    std::cout << "n: " << instance.n() << std::endl;
    std::cout << "m: " << instance.m() << std::endl;

    return 0;
}
