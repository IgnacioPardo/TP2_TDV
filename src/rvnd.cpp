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

    int i = 0;

    while (true){

        auto now = std::chrono::high_resolution_clock::now();
        std::cout << "Iteracion: " << i << " Last Cost: " << sol.cost() << " Best Costo: " << best_cost << " Tiempo: " << std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() << std::endl;

        if (i % (its/2) == 0){
            // Reiniciar solucion

            if (i % (its) == 0){
                // La mitad de las veces, se inicializa con BinPackingRandomized
                BinPackingRandomized binpacking_randomized(this->_instance);
                binpacking_randomized.solve();

                sol = binpacking_randomized.get_solution();
                std::cout << "BinPackingRandomized Init cost: " << sol.cost() << std::endl;

                if (sol.cost() < best_cost || best_cost == 0){
                    best_cost = sol.cost();
                    best_sol = sol;
                }
            }
            else{
                // La otra mitad de las veces, se inicializa con GreedyMinCost
                GreedyRandomized greedy(this->_instance);
                greedy.solve();

                sol = greedy.get_solution();
                std::cout << "GreedyMinCost Init cost: " << sol.cost() << std::endl;
                
                if (sol.cost() < best_cost){
                    best_cost = sol.cost();
                    best_sol = sol;
                }
            }
        }
        else if (i % (its/2) != 0){
            // Destruir solucion si no se reinicia
            RandomDestroyer random_destroyer(this->_instance);
            random_destroyer.set_is_unassigner(true);
            random_destroyer.solve(sol);
            sol = random_destroyer.get_solution();
        }

        double prev_cost = 0;
        int v = 0;
        // Mientras se mejore la solucion
        while(sol.cost() < prev_cost || prev_cost == 0){

            prev_cost = sol.cost();

            Swap swap(this->_instance);
            swap.set_solution(sol);
            std::vector<std::tuple<int, int, int, int, double>> swap_neighborhood = swap.neighbourhood();

            if (swap_neighborhood.size() == 0){
                break;
            }

            // ordenar por costo
            // std::sort(swap_neighborhood.begin(), swap_neighborhood.end(), [](std::tuple<int, int, int, int, double> a, std::tuple<int, int, int, int, double> b) { return std::get<4>(a) < std::get<4>(b); });
            
            swap.do_swap(std::get<0>(swap_neighborhood[0]), std::get<1>(swap_neighborhood[0]), std::get<2>(swap_neighborhood[0]), std::get<3>(swap_neighborhood[0]));
            sol = swap.get_solution();
            
            Relocate relocate(this->_instance);
            relocate.set_solution(sol);
            
            std::vector<std::tuple<int, int, double>> relocate_neighborhood = relocate.neighbourhood();

            if (relocate_neighborhood.size() == 0){
                break;
            }

            // ordenar por costo
            // std::sort(relocate_neighborhood.begin(), relocate_neighborhood.end(), [](std::tuple<int, int, double> a, std::tuple<int, int, double> b) { return std::get<2>(a) < std::get<2>(b); });
            
            relocate.do_relocation(std::get<0>(relocate_neighborhood[0]), std::get<1>(relocate_neighborhood[0]));
            sol = relocate.get_solution();

            if (sol.cost() < best_cost){
                best_cost = sol.cost();
                best_sol = sol;
            }
            v++;
        }

    i++;
    }

    this->_solution = best_sol;

    // timer
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(time);
}
