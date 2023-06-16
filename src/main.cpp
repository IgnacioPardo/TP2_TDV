#include <string>
#include <iostream>

#include "gap_instance.h"

#include "greedy_mincost.h"
#include "binpacking.h"

int main(int argc, char** argv) {
    //std::string filename = "instances/gap/gap_b/b05100";
    std::string filename = "instances/gap/gap_a/a05100";
    std::cout << "Reading file " << filename << std::endl;

    // Aca empieza la magia 🪄

    GapInstance instance(filename);

    std::cout << "n: " << instance.n() << std::endl;
    std::cout << "m: " << instance.m() << std::endl;

    // for (int i = 0; i < instance.m(); i++) {
    //     std::cout << "capacidad " << i << ": " << instance.capacidad(i) << std::endl;
    // }

    // GreedyMinCost greedy(instance);

    // greedy.solve();

    // GapSolution solution = greedy.get_solution();

    // std::cout << solution << std::endl;

    BinPacking binpacking(instance);

    binpacking.solve();

    GapSolution solution = binpacking.get_solution();

    std::cout << solution << std::endl;

    return 0;
}
