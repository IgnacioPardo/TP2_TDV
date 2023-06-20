#include <string>
#include <iostream>

#include "gap_instance.h"
#include "gap_solution.h"

#include "greedy_mincost.h"
#include "binpacking.h"
#include "swap.h"
#include "relocate.h"


int main(int argc, char** argv) {
    // std::string filename = "instances/gap/gap_a/a05100";
    // std::string filename = "instances/gap/gap_b/b05100";
    // std::string filename = "instances/gap/gap_e/e801600";
    std::string filename = "instances/real/real_instance";

    std::cout << "Reading file " << filename << std::endl;

    // Aca empieza la magia 🪄

    GapInstance instance(filename);

    std::cout << "Vendedores: " << instance.n() << std::endl;
    std::cout << "Depósitos: " << instance.m() << std::endl;

    // for (int i = 0; i < instance.m(); i++) {
    //     std::cout << "capacidad " << i << ": " << instance.capacidad(i) << std::endl;
    // }

    std::cout << std::endl;

    // --------------------------------- GREEDY SOLUTION ---------------------------------------

    GreedyMinCost greedy(instance);

    greedy.solve();

    GapSolution greedy_solution = greedy.get_solution();

    std::cout << "Greedy solution cost: " << greedy_solution.cost() << std::endl;
    std::cout << "Greedy solution time: " << greedy_solution.time() << std::endl;
    
    std::cout << std::endl;

    // ------------------------------ BINPACKING SOLUTION--------------------------------------
    
    BinPacking binpacking(instance);

    binpacking.solve();

    GapSolution binpacking_solution = binpacking.get_solution();

    std::cout << "Bin Packing solution cost: " << binpacking_solution.cost() << std::endl;
    std::cout << "Bin Packing solution time: " << binpacking_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  SWAP LOCAL SOLUTION ------------------------------------------

    Swap swap(instance);

    swap.solve();

    GapSolution swap_solution = swap.get_solution();

    std::cout << "Swap (Greedy) solution cost: " << swap_solution.cost() << std::endl;
    std::cout << "Swap (Greedy) solution time: " << swap_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  RELOCATE LOCAL SOLUTION ------------------------------------------

    Relocate relocate(instance);

    relocate.solve();

    GapSolution relocate_solution = relocate.get_solution();

    std::cout << "Relocate (Greedy) solution cost: " << relocate_solution.cost() << std::endl;
    std::cout << "Relocate (Greedy) solution time: " << relocate_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  RELOCATE SWAP LOCAL SOLUTION ------------------------------------------

    Relocate relocate_swap(instance);

    relocate_swap.solve(swap_solution);

    GapSolution relocate_swap_solution = relocate_swap.get_solution();

    std::cout << "Relocate (Swap (Greedy)) solution cost: " << relocate_swap_solution.cost() << std::endl;
    std::cout << "Relocate (Swap (Greedy)) solution time: " << relocate_swap_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  SWAP RELOCATE LOCAL SOLUTION ------------------------------------------

    Swap swap_relocate(instance);

    swap_relocate.solve(relocate_solution);

    GapSolution swap_relocate_solution = swap_relocate.get_solution();

    std::cout << "Swap (Relocate (Swap (Greedy))) solution cost: " << swap_relocate_solution.cost() << std::endl;
    std::cout << "Swap (Relocate (Swap (Greedy))) solution time: " << swap_relocate_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  RELOCATE SWAP RELOCATE LOCAL SOLUTION ------------------------------------------

    Relocate relocate_swap_relocate(instance);

    relocate_swap_relocate.solve(swap_relocate_solution);

    GapSolution relocate_swap_relocate_solution = relocate_swap_relocate.get_solution();

    std::cout << "Relocate (Swap (Relocate (Swap (Greedy)))) solution cost: " << relocate_swap_relocate_solution.cost() << std::endl;
    std::cout << "Relocate (Swap (Relocate (Swap (Greedy)))) solution time: " << relocate_swap_relocate_solution.time() << std::endl;
    std::cout << std::endl;

    return 0;
}
