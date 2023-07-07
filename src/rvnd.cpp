#include "rvnd.h"

#include <iostream>
#include <vector>
#include <random>

#include "gap_solution.h"
#include "gap_instance.h"

#include "random_destroyer.h"
#include "greedy_randomized.h"
#include "binpacking_randomized.h"
#include "relocate.h"
#include "swap.h"


RVND::~RVND() {}

std::string RVND::get_name() const {
    return "RandomizedVariableNeighborhoodDescent";
}

void RVND::solve(){

    // timer
    auto start = std::chrono::high_resolution_clock::now();

    GapSolution sol = this->_solution;
    GapSolution best_sol;
    double best_cost = 0;

    int its = 10;

    for (int i = 0; i < 1; i++){
        // std::cout << "Iteration: " << i << " Best cost: " << best_cost << std::endl;

        // std::cout << i << " " << its << " " << (its/2) << " " << (i % (its/2)) << std::endl;
        if (i % (its/2) == 0){
            if (i % (its) == 0){
                // std::cout << "BinPackingRandomized cost: ";

                BinPackingRandomized binpacking_randomized(this->_instance);

                binpacking_randomized.solve();

                sol = binpacking_randomized.get_solution();

                if (sol.cost() < best_cost || best_cost == 0){
                    best_cost = sol.cost();
                    best_sol = sol.copy();
                }
                // std::cout << sol.cost() << std::endl;
            }
            else{
                // std::cout << "GreedyMinCost cost: ";
                GreedyMinCost greedy(this->_instance);
                greedy.solve();
                sol = greedy.get_solution();
                if (sol.cost() < best_cost){
                    best_cost = sol.cost();
                    best_sol = sol.copy();
                }
                // std::cout << sol.cost() << std::endl;
            }
        }
        else if (i % (its/2) != 0){
            // std::cout << "Random Destroyer cost: ";
            RandomDestroyer random_destroyer(this->_instance);
            random_destroyer.set_is_unassigner(true);
            random_destroyer.solve(sol);
            sol = random_destroyer.get_solution();
            // std::cout << sol.cost() << std::endl;
        }

        double prev_cost = 0;
        int v = 0;
        while(sol.cost() < prev_cost || prev_cost == 0){

            // std::cout << "Iteration: " << v++ << std::endl;
            prev_cost = sol.cost();

            Swap swap(this->_instance);
            swap.set_solution(sol);
            std::vector<std::tuple<int, int, int, int, double>> swap_neighborhood = swap.neighbourhood();

            if (swap_neighborhood.size() == 0){
                break;
            }

            // ordenar por costo
            std::sort(swap_neighborhood.begin(), swap_neighborhood.end(), [](std::tuple<int, int, int, int, double> a, std::tuple<int, int, int, int, double> b) { return std::get<4>(a) < std::get<4>(b); });
            swap.do_swap(std::get<0>(swap_neighborhood[0]), std::get<1>(swap_neighborhood[0]), std::get<2>(swap_neighborhood[0]), std::get<3>(swap_neighborhood[0]));
            sol = swap.get_solution();
            Relocate relocate(this->_instance);
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
                best_sol = sol.copy();
            }
            v++;
        }
        // std::cout << "Found cost: " << sol.cost() << std::endl;
    }

    this->_solution = best_sol;

    // timer
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(time);
}
