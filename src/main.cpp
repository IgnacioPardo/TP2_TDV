#include <string>
#include <iostream>
#include <filesystem>

#include "gap_instance.h"
#include "gap_solution.h"

#include "greedy_mincost.h"
#include "binpacking.h"
#include "swap.h"
#include "relocate.h"

void results_to_csv(){

    if(std::filesystem::exists("output/results.csv"))
        std::filesystem::remove("output/results.csv");

    std::ofstream log_file;
    std::string log_results_filename = "output/results.csv";
    log_file.open(log_results_filename, std::ios_base::app);
    log_file << "Filename,Depositos,Vendedores,Greedy_cost,Greedy_time,Binpacking_cost,Binpacking_time,Swap(Greedy)_cost,Swap(Greedy)_time,Relocate(Bin Packing)_cost,Relocate(Bin Packing)_time,Metaheuristic_cost,Metaheuristic_time" << std::endl;

    std:: vector<std::string> index = {"gap/gap_a", "gap/gap_b", "gap/gap_e", "real"};
    
    for (int i = 0; i < index.size(); i++){
        for (const auto & entry : std::filesystem::directory_iterator("instances/" + index[i])){

            std::string filename = entry.path().string();

            if(filename.find(".") != std::string::npos)
                continue;

            GapInstance instance(filename);

            // ------------------------------------- GREEDY SOLUTION ---------------------------------------------------------------------------

            GreedyMinCost greedy(instance);

            greedy.solve();

            GapSolution greedy_solution = greedy.get_solution();

            // ----------------------------------- BINPACKING SOLUTION---------------------------------------------------------------------------

            BinPacking binpacking(instance);

            binpacking.solve();

            GapSolution binpacking_solution = binpacking.get_solution();

            // ----------------------------------  SWAP LOCAL SOLUTION ------------------------------------------------------------------------

            Swap swap(instance);

            swap.solve(greedy_solution.copy());

            GapSolution swap_solution = swap.get_solution();

            // ----------------------------------  RELOCATE BIN PACKING LOCAL SOLUTION -------------------------------------------------------------------

            Relocate relocate(instance);

            relocate.solve(binpacking_solution.copy());

            GapSolution relocate_solution = relocate.get_solution();

            // --------------------------------------  METAHEURISTICA ---------------------------------------------------------------

            Relocate relocate_swap(instance);

            relocate_swap.solve(swap_solution);

            GapSolution metaheuristica = relocate_swap.get_solution();
            
            // si el filename contiene "real_instance" no tomar los ultimos 6 caracteres
            if(filename != "instances/real/real_instance")
                log_file << filename.substr(filename.size() - 6) << "," << instance.m()<< "," << instance.n() << "," << greedy_solution.cost() << "," << greedy_solution.time() << "," << binpacking_solution.cost() << "," << binpacking_solution.time() << "," << swap_solution.cost() << "," << swap_solution.time() << "," << relocate_solution.cost() << "," << relocate_solution.time() << "," << metaheuristica.cost() << "," << metaheuristica.time() << std::endl;
            else{
                log_file << filename.substr(filename.size() - 13) << "," << instance.m()<< "," << instance.n() << "," << greedy_solution.cost() << "," << greedy_solution.time() << "," << binpacking_solution.cost() << "," << binpacking_solution.time() << "," << swap_solution.cost() << "," << swap_solution.time() << "," << relocate_solution.cost() << "," << relocate_solution.time() << "," << metaheuristica.cost() << "," << metaheuristica.time() << std::endl;
            }
        
        }
    }

}

void tester(){
    // std::string filename = "instances/gap/gap_a/a05100";
    std::string filename = "instances/gap/gap_b/b20200";
    // std::string filename = "instances/gap/gap_b/b05100";
    // std::string filename = "instances/gap/gap_e/e801600";
    // std::string filename = "instances/real/real_instance";

    std::cout << "Reading file " << filename << std::endl;

    // Aca empieza la magia 🪄

    GapInstance instance(filename);

    std::cout << "Vendedores: " << instance.n() << std::endl;
    std::cout << "Depósitos: " << instance.m() << std::endl;

    std::cout << std::endl;

    // --------------------------------- GREEDY SOLUTION ---------------------------------------------------------------------------

    GreedyMinCost greedy(instance);

    greedy.solve();

    GapSolution greedy_solution = greedy.get_solution();
    
    std::cout << "Greedy solution cost: " << greedy_solution.cost() << std::endl;
    std::cout << "Greedy solution time: " << greedy_solution.time() << std::endl;
    
    std::cout << std::endl;


    // -------------------------------- BINPACKING SOLUTION---------------------------------------------------------------------------
    
    BinPacking binpacking(instance);

    binpacking.solve();

    GapSolution binpacking_solution = binpacking.get_solution();

    std::cout << "Bin Packing solution cost: " << binpacking_solution.cost() << std::endl;
    std::cout << "Bin Packing solution time: " << binpacking_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  SWAP GREEDY LOCAL SOLUTION ------------------------------------------------------------------------

    Swap swap_g(instance);

    swap_g.solve(greedy_solution.copy());

    GapSolution swap_g_solution = swap_g.get_solution();

    std::cout << "Swap (Greedy) solution cost: " << swap_g_solution.cost() << std::endl;
    std::cout << "Swap (Greedy) solution time: " << swap_g_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  SWAP BIN PACKING LOCAL SOLUTION ------------------------------------------------------------------------

    // Swap swap_bp(instance);

    // swap_bp.solve(binpacking_solution.copy());

    // GapSolution swap_bp_solution = swap_bp.get_solution();

    // std::cout << "Swap (Bin Packing) solution cost: " << swap_bp_solution.cost() << std::endl;
    // std::cout << "Swap (Bin Packing) solution time: " << swap_bp_solution.time() << std::endl;

    // std::cout << std::endl;

    // --------------------------------  RELOCATE GREEDY LOCAL SOLUTION -------------------------------------------------------------------

    Relocate relocate(instance);

    relocate.solve(greedy_solution.copy());

    GapSolution relocate_greedy_solution = relocate.get_solution();

    std::cout << "Relocate (Greedy) solution cost: " << relocate_greedy_solution.cost() << std::endl;
    std::cout << "Relocate (Greedy) solution time: " << relocate_greedy_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  RELOCATE BIN PACKING LOCAL SOLUTION -------------------------------------------------------------------

    Relocate relocate_bp(instance);

    relocate_bp.solve(binpacking_solution.copy());

    GapSolution relocate_binpacking_solution = relocate_bp.get_solution();

    std::cout << "Relocate (Bin Packing) solution cost: " << relocate_binpacking_solution.cost() << std::endl;
    std::cout << "Relocate (Bin Packing) solution time: " << relocate_binpacking_solution.time() << std::endl;

    std::cout << std::endl;

    // --------------------------------  RELOCATE SWAP LOCAL SOLUTION ---------------------------------------------------------------

    /* Relocate relocate_swap(instance);

    relocate_swap.solve(swap_solution);

    GapSolution relocate_swap_solution = relocate_swap.get_solution();

    std::cout << "Relocate (Swap (Greedy)) solution cost: " << relocate_swap_solution.cost() << std::endl;
    std::cout << "Relocate (Swap (Greedy)) solution time: " << relocate_swap_solution.time() << std::endl;

    std::cout << std::endl; */

    // --------------------------------  SWAP RELOCATE SWAP LOCAL SOLUTION ---------------------------------------------------------------

    /* Swap swap_relocate_swap(instance);

    swap_relocate_swap.solve(relocate_swap_solution);

    GapSolution swap_relocate_swap_solution = swap_relocate_swap.get_solution();

    std::cout << "Swap (Relocate (Swap (Greedy))) solution cost: " << swap_relocate_swap_solution.cost() << std::endl;
    std::cout << "Swap (Relocate (Swap (Greedy))) solution time: " << swap_relocate_swap_solution.time() << std::endl;

    std::cout << std::endl; */

    // -------- ------------------------  RELOCATE SWAP RELOCATE SWAP LOCAL SOLUTION ------------------------------------------------------

    /* Relocate relocate_swap_relocate_swap(instance);

    relocate_swap_relocate_swap.solve(swap_relocate_swap_solution);

    GapSolution relocate_swap_relocate_swap_solution = relocate_swap_relocate_swap.get_solution();

    std::cout << "Relocate (Swap (Relocate (Swap (Greedy)))) solution cost: " << relocate_swap_relocate_swap_solution.cost() << std::endl;
    std::cout << "Relocate (Swap (Relocate (Swap (Greedy)))) solution time: " << relocate_swap_relocate_swap_solution.time() << std::endl;
    std::cout << std::endl; */
}

int main(int argc, char** argv) {

    // results_to_csv();

    tester();
    
    return 0;
}
