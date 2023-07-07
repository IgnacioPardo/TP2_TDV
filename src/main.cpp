#include <string>
#include <iostream>
#include <filesystem>

#include "gap_instance.h"
#include "gap_solution.h"

#include "greedy_mincost.h"
#include "binpacking.h"
#include "swap.h"
#include "relocate.h"
#include "random_destroyer.h"

#include "tabu.h"
#include "rvnd.h"

#include "greedy_randomized.h"
#include "binpacking_randomized.h"

void results_to_csv(){
    /*
    *   Funcion que genera un archivo csv con los resultados de las heurísticas.
    *   El archivo se genera en la carpeta output.
    *   Si el archivo ya existe, se sobreescribe.
    */

    if(std::filesystem::exists("output/results.csv"))
        std::filesystem::remove("output/results.csv");

    std::ofstream log_file;
    std::string log_results_filename = "output/results.csv";
    log_file.open(log_results_filename, std::ios_base::app);
    log_file << "Filename,Depositos,Vendedores,Greedy_cost,Greedy_time,Greedy_free,Binpacking_cost,Binpacking_time,Binpacking_free,Swap(Greedy)_cost,Swap(Greedy)_time,Swap(Greedy)_free,Swap(Bin Packing)_cost,Swap(Bin Packing)_time,Swap(Bin Packing)_free,Relocate(Greedy)_cost,Relocate(Greedy)_time,Relocate(Greedy)_free,Relocate(Bin Packing)_cost,Relocate(Bin Packing)_time,Relocate(Bin Packing)_free,Swap+Relocate(Greedy)_cost,Swap+Relocate(Greedy)_time,Swap+Relocate(Greedy)_free,Swap+Relocate(Bin Packing)_cost,Swap+Relocate(Bin Packing)_time,Swap+Relocate(Bin Packing)_free,Relocate+Swap(Greedy)_cost,Relocate+Swap(Greedy)_time,Relocate+Swap(Greedy)_free,Relocate+Swap(Bin Packing)_cost,Relocate+Swap(Bin Packing)_time,Relocate+Swap(Bin Packing)_free,Tabu_cost,Tabu_time,Tabu_free,RVND_cost,RVND_time,RVND_free\n";

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

            // ----------------------------------  SWAP GREEDY LOCAL SOLUTION ------------------------------------------------------------------------

            Swap swap_g(instance);

            swap_g.solve(greedy_solution.copy());

            GapSolution swap_solution = swap_g.get_solution();

            // ----------------------------------  RELOCATE GREEDY LOCAL SOLUTION -------------------------------------------------------------------

            Relocate relocate_g(instance);

            relocate_g.solve(greedy_solution.copy());

            GapSolution relocate_solution = relocate_g.get_solution();

            // --------------------------------------  SWAP BINPACKING LOCAL SOLUTION --------------------------------------------------------

            Swap swap_b(instance);

            swap_b.solve(binpacking_solution.copy());

            GapSolution swap_solution_b = swap_b.get_solution();

            // --------------------------------------  RELOCATE BINPACKING LOCAL SOLUTION --------------------------------------------------------

            Relocate relocate_b(instance);

            relocate_b.solve(binpacking_solution.copy());

            GapSolution relocate_solution_b = relocate_b.get_solution();

            // --------------------------------------  SWAP + RELOCATE GREEDY LOCAL SOLUTION --------------------------------------------------------

            Swap swap_r_g(instance);

            swap_r_g.solve(relocate_solution.copy());

            GapSolution swap_relocate_solution_g = swap_r_g.get_solution();

            // --------------------------------------  RELOCATE + SWAP GREEDY LOCAL SOLUTION --------------------------------------------------------

            Relocate relocate_s_g(instance);

            relocate_s_g.solve(swap_solution.copy());

            GapSolution relocate_swap_solution_g = relocate_s_g.get_solution();
            
            // --------------------------------------  SWAP + RELOCATE BINPACKING LOCAL SOLUTION --------------------------------------------------------

            Swap swap_r_b(instance);

            swap_r_b.solve(relocate_solution_b.copy());

            GapSolution swap_relocate_solution_b = swap_r_b.get_solution();

            // --------------------------------------  RELOCATE + SWAP BINPACKING LOCAL SOLUTION --------------------------------------------------------

            Relocate relocate_s_b(instance);

            relocate_s_b.solve(swap_solution_b.copy());

            GapSolution relocate_swap_solution_b = relocate_s_b.get_solution();

            // --------------------------------------  TABU METAHEURISTICA ------------------------------------------------------------------------

            Tabu tabu(instance);

            tabu.solve();

            GapSolution tabu_solution = tabu.get_solution();

            // --------------------------------------  RVND METAHEURISTICA ------------------------------------------------------------------------

            RVND rvnd(instance);

            rvnd.solve();

            GapSolution rvnd_solution = rvnd.get_solution();

            // Split filename to get the instance name            
            std::string instance_name = filename.substr(filename.find_last_of("/") + 1);
            log_file << instance_name;
            log_file << "," << instance.m() << "," << instance.n();
            log_file << "," << greedy_solution.cost() << "," << greedy_solution.time() << "," << greedy_solution.unassigned_vendors().size();
            log_file << "," << binpacking_solution.cost() << "," << binpacking_solution.time() << "," << binpacking_solution.unassigned_vendors().size();
            log_file << "," << swap_solution.cost() << "," << swap_solution.time() << "," << swap_solution.unassigned_vendors().size();
            log_file << "," << swap_solution_b.cost() << "," << swap_solution_b.time() << "," << swap_solution_b.unassigned_vendors().size();
            log_file << "," << relocate_solution.cost() << "," << relocate_solution.time() << "," << relocate_solution.unassigned_vendors().size();
            log_file << "," << relocate_solution_b.cost() << "," << relocate_solution_b.time() << "," << relocate_solution_b.unassigned_vendors().size();
            log_file << "," << swap_relocate_solution_g.cost() << "," << swap_relocate_solution_g.time() << "," << swap_relocate_solution_g.unassigned_vendors().size();
            log_file << "," << swap_relocate_solution_b.cost() << "," << swap_relocate_solution_b.time() << "," << swap_relocate_solution_b.unassigned_vendors().size();
            log_file << "," << relocate_swap_solution_g.cost() << "," << relocate_swap_solution_g.time() << "," << relocate_swap_solution_g.unassigned_vendors().size();
            log_file << "," << relocate_swap_solution_b.cost() << "," << relocate_swap_solution_b.time() << "," << relocate_swap_solution_b.unassigned_vendors().size();
            log_file << "," << tabu_solution.cost() << "," << tabu_solution.time() << "," << tabu_solution.unassigned_vendors().size();
            log_file << "," << rvnd_solution.cost() << "," << rvnd_solution.time() << "," << rvnd_solution.unassigned_vendors().size();
            log_file << std::endl;
        }
    }

}

void tester(){
    /*
    *   Tester para probar las heurísticas
    */

    // std::string filename = "instances/gap/gap_a/a05100";
    // std::string filename = "instances/gap/gap_b/b10200";
    std::string filename = "instances/gap/gap_b/b05100";
    // std::string filename = "instances/gap/gap_e/e801600";
    // std::string filename = "instances/real/real_instance";

    std::cout << "Reading file " << filename << std::endl;

    GapInstance instance(filename);

    std::cout << "Vendedores: " << instance.n() << std::endl;
    std::cout << "Depósitos: " << instance.m() << std::endl;

    std::cout << std::endl;

    RVND rvnd(instance);

    rvnd.solve();

    GapSolution rvnd_solution = rvnd.get_solution();
    rvnd_solution.recalc_cost();

    std::cout << "RVND solution cost: " << rvnd_solution.cost() << std::endl;
    std::cout << "RVND solution time: " << rvnd_solution.time() << std::endl;

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

    Swap swap_bp(instance);

    swap_bp.solve(binpacking_solution.copy());

    GapSolution swap_bp_solution = swap_bp.get_solution();

    std::cout << "Swap (Bin Packing) solution cost: " << swap_bp_solution.cost() << std::endl;
    std::cout << "Swap (Bin Packing) solution time: " << swap_bp_solution.time() << std::endl;

    std::cout << std::endl;

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

    // --------------------------------  RELOCATE SWAP GREEDY LOCAL SOLUTION ---------------------------------------------------------------

    Relocate relocate_swap_g(instance);

    relocate_swap_g.solve(swap_g_solution.copy());

    GapSolution relocate_swap_g_solution = relocate_swap_g.get_solution();

    std::cout << "Relocate (Swap (Greedy)) solution cost: " << relocate_swap_g_solution.cost() << std::endl;
    std::cout << "Relocate (Swap (Greedy)) solution time: " << relocate_swap_g_solution.time() << std::endl;

    // --------------------------------  SWAP RELOCATE GREEDY LOCAL SOLUTION ---------------------------------------------------------------

    Swap swap_relocate_g(instance);

    swap_relocate_g.solve(relocate_greedy_solution.copy());

    GapSolution swap_relocate_g_solution = swap_relocate_g.get_solution();

    std::cout << "Swap (Relocate (Greedy)) solution cost: " << swap_relocate_g_solution.cost() << std::endl;
    std::cout << "Swap (Relocate (Greedy)) solution time: " << swap_relocate_g_solution.time() << std::endl;

    // -------- ------------------------  RELOCATE SWAP RELOCATE SWAP GREEDY LOCAL SOLUTION ------------------------------------------------------

    Relocate relocate_swap_relocate_swap_g(instance);

    relocate_swap_relocate_swap_g.solve(swap_relocate_g_solution.copy());

    GapSolution relocate_swap_relocate_swap_g_solution = relocate_swap_relocate_swap_g.get_solution();

    std::cout << "Relocate (Swap (Relocate (Swap))) solution cost: " << relocate_swap_relocate_swap_g_solution.cost() << std::endl;
    std::cout << "Relocate (Swap (Relocate (Swap))) solution time: " << relocate_swap_relocate_swap_g_solution.time() << std::endl;

    // --------------------------------  Tabu ---------------------------------------------------------------

    Tabu tabu(instance);

    tabu.solve();

    GapSolution tabu_solution = tabu.get_solution();

    std::cout << "Tabu solution cost: " << tabu_solution.cost() << std::endl;
    std::cout << "Tabu solution time: " << tabu_solution.time() << std::endl;
}

void other_Tabuheuristic(){
    
    std::string filename = "instances/real/real_instance";

    std::cout << "Reading file " << filename << std::endl;

    GapInstance instance(filename);

    // timer
    auto start = std::chrono::high_resolution_clock::now();
    
    GapSolution sol = GapSolution(instance);
    double best_cost = 0;

    int its = 10;

    for (int i = 0; i < 1; i++){
        std::cout << "Iteration: " << i << " Best cost: " << best_cost << std::endl;
        
        if (i % (its/2) == 0){
            std::cout << "Restart ";
            if (i % (its) == 0){
                std::cout << "BinPackingRandomized cost: ";
                BinPackingRandomized binpacking_randomized(instance);
                binpacking_randomized.solve();
                sol = binpacking_randomized.get_solution();
                if (sol.cost() < best_cost || best_cost == 0){
                    best_cost = sol.cost();
                }
                std::cout << sol.cost() << std::endl;
            }
            else{
                std::cout << "GreedyMinCost cost: ";
                GreedyMinCost greedy(instance);
                greedy.solve();
                sol = greedy.get_solution();
                if (sol.cost() < best_cost){
                    best_cost = sol.cost();
                }
                std::cout << sol.cost() << std::endl;
            }
        }
        else if (i % (its/2) != 0){
            std::cout << "Random Destroyer cost: ";
            RandomDestroyer random_destroyer(instance);
            random_destroyer.solve(sol);
            sol = random_destroyer.get_solution();
            std::cout << sol.cost() << std::endl;
        }

        double prev_cost = 0;
        int v = 0;
        while(sol.cost() < prev_cost || prev_cost == 0){

            // std::cout << "Iteration: " << v++ << std::endl;
            prev_cost = sol.cost();

            Swap swap(instance);
            swap.set_solution(sol);
            std::vector<std::tuple<int, int, int, int, double>> swap_neighborhood = swap.neighbourhood();

            if (swap_neighborhood.size() == 0){
                break;
            }

            // ordenar por costo
            std::sort(swap_neighborhood.begin(), swap_neighborhood.end(), [](std::tuple<int, int, int, int, double> a, std::tuple<int, int, int, int, double> b) { return std::get<4>(a) < std::get<4>(b); });
            swap.do_swap(std::get<0>(swap_neighborhood[0]), std::get<1>(swap_neighborhood[0]), std::get<2>(swap_neighborhood[0]), std::get<3>(swap_neighborhood[0]));
            sol = swap.get_solution();
            Relocate relocate(instance);
            relocate.set_solution(sol);
            std::vector<std::tuple<int, int, double>> relocate_neighborhood = relocate.neighbourhood();

            if (relocate_neighborhood.size() == 0){
                break;
            }

            // ordenar por costo
            std::sort(relocate_neighborhood.begin(), relocate_neighborhood.end(), [](std::tuple<int, int, double> a, std::tuple<int, int, double> b) { return std::get<2>(a) < std::get<2>(b); });
            relocate.do_relocation(std::get<0>(relocate_neighborhood[0]), std::get<1>(relocate_neighborhood[0]));
            sol = relocate.get_solution();

            if (sol.cost() < best_cost){
                best_cost = sol.cost();
            }
            v++;
        }
        std::cout << "Found cost: " << sol.cost() << std::endl;
    }

    std::cout << "Sol cost: " << sol.cost() << std::endl;

    std::cout << "Best cost: " << best_cost << std::endl;

    // timer
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Time: " << time << std::endl;
}

int main(int argc, char** argv) {
    
    // Aca empieza la magia 🪄
    
    results_to_csv();

    // tester();

    // other_Tabuheuristic();

    return 0;
}
