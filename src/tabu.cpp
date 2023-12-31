#include "tabu.h"

#include <iostream>
#include <vector>
#include <random>

#include "gap_solution.h"
#include "gap_instance.h"

#include "random_destroyer.h"
#include "greedy_mincost.h"
#include "greedy_randomized.h"
#include "binpacking_randomized.h"
#include "relocate.h"
#include "swap.h"

Tabu::~Tabu() {}

std::string Tabu::get_name() const {
    return "Tabu";
}

void Tabu::solve() {
    /*
    *  Busqeda Tabú sobre soluciones Golosas y Bin Packing, aleatorias y destruidas con Relocate y Swap
    */


    //start timer

    auto start = std::chrono::high_resolution_clock::now();
    
    int iter_count = 10;
    int max_tries = 10;
    int cutoff = 30;
    int neighbourhood_count = 10;

    std::vector<GapSolution> solutions = std::vector<GapSolution>();
    std::random_device rd;
    std::mt19937 gen(rd());

    // Optamos por inicializar la solucion con BinPacking y no con Greedy por la experimentación llevada a cabo.
    // GreedyMinCost greedy = GreedyMinCost(this->_instance);
    // std::cout << "Greedy Solve" << std::endl;
    // greedy.solve();
    // this->_solution = greedy.get_solution();

    BinPacking bin_packing = BinPacking(this->_instance);
    bin_packing.solve();
    this->_solution = bin_packing.get_solution();

    int last_neighbourhood_size = 0;

    int tries = 0;

    for (int i = 0; i < iter_count; i++){
        
        if (i > cutoff){
            // Si las ultimas 3 soluciones son peores o iguales que la anterior, se corta la ejecucion
            int stop_count = 0;
            for (int j = 0; j < 3; j++){
                if (solutions[i - j].cost() >= solutions[i - j - 1].cost()){
                    stop_count++;
                }
            }
            if (stop_count == 3){
                
                if (tries > max_tries){
                    break;
                }
                else{
                    
                    // Resetear solucion
                    // Alternate between greedy and binpacking

                    if (tries % 2 == 0){    
                        GreedyRandomized greedy_randomized = GreedyRandomized(this->_instance);
                        greedy_randomized.solve();
                        this->_solution = greedy_randomized.get_solution();
                    }
                    else{
                        BinPackingRandomized bin_packing_randomized = BinPackingRandomized(this->_instance);
                        bin_packing_randomized.solve();
                        this->_solution = bin_packing_randomized.get_solution();
                    }
                }
            }
        }

        std::uniform_int_distribution<> destroy_dis(1, 10);

        // Con probabilidad 0.3 se destruye la solucion
        if (destroy_dis(gen) <= 3) {

            RandomDestroyer destroyer = RandomDestroyer(this->_instance);
            destroyer.solve(this->_solution);
            this->_solution = destroyer.get_solution();
        }

        // Explorar el vecindario de la solucion con Relocate de forma exhaustiva: Best Improvement
        Relocate relocate = Relocate(this->_instance);
        relocate.solve(this->_solution);
        this->_solution = relocate.get_solution();

        // Explorar el vecindario de la solucion con Swap de forma Tabú
        Swap swap = Swap(this->_instance);
        swap.set_solution(this->_solution);

        for (int j = 0; j < neighbourhood_count; j++) {

            // Encontrar vecindario de la solucion con Swap
            std::vector<std::tuple<int, int, int, int, double>> tabu_neighbours = swap.neighbourhood();

            if (tabu_neighbours.size() == 0) {
                break;
            }
            
            // std::sort(tabu_neighbours.begin(), tabu_neighbours.end(), [](const std::tuple<int, int, int, int, double> &a, const std::tuple<int, int, int, int, double> &b) {
            //     return std::get<4>(a) < std::get<4>(b);
            // });

            std::tuple<int, int, int, int, double> best_swap = tabu_neighbours[0];

            last_neighbourhood_size = tabu_neighbours.size();

            // Si la mejor solucion del vecindario es peor que la actual con mas de un 40% de diferencia, se corta la ejecucion
            if (std::get<4>(best_swap) - this->_solution.cost() > 0.4 * this->_solution.cost()) {
                break;
            }

            int v1 = std::get<0>(best_swap);
            int v2 = std::get<1>(best_swap);
            int d1 = std::get<2>(best_swap);
            int d2 = std::get<3>(best_swap);

            swap.do_swap(v1, v2, d1, d2);

            // Efectuar el swap en la lista tabu

        }
        
        this->_solution = swap.get_solution();
        this->_solution.recalc_cost();

        solutions.push_back(this->_solution.copy());
    }

    // Ordenar los minimos locales por costo
    std::sort(solutions.begin(), solutions.end(), [](const GapSolution &a, const GapSolution &b) {
        return a < b;
    });


    // Tomar la mejor solucion
    this->_solution = solutions[0];

    //end timer
    auto end = std::chrono::high_resolution_clock::now();
    this->_solution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    this->_solution.set_time(this->_solution_time);
}
