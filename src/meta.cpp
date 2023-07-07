#include "meta.h"
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

Meta::~Meta() {}

std::string Meta::get_name() const {
    return "Meta";
}

void Meta::solve() {
    /*
    *  Tabú Swap with Random Destruction
    */

    int iter_count = 10;
    int max_tries = 10;
    int cutoff = 30;
    int neighbourhood_count = 10;

    std::vector<GapSolution> solutions = std::vector<GapSolution>();

    //start timer

    auto start = std::chrono::high_resolution_clock::now();
    std::random_device rd;
    std::mt19937 gen(rd());

    // std::cout << "Greedy init" << std::endl;

    // GreedyMinCost greedy = GreedyMinCost(this->_instance);
    // std::cout << "Greedy Solve" << std::endl;
    // greedy.solve();
    // this->_solution = greedy.get_solution();


    BinPacking bin_packing = BinPacking(this->_instance);
    bin_packing.solve();
    this->_solution = bin_packing.get_solution();

    std::cout << "BinPacking Init Cost: " << this->_solution.cost() << std::endl;
    
    // std::cout << "Greedy Solved" << std::endl;

    // std::cout << "Touring" << std::endl;

    int last_neighbourhood_size = 0;

    int tries = 0;

    for (int i = 0; i < iter_count; i++){
        
        if (i > cutoff){
            // If last 3 solutions are the worst than i-3, stop
            bool stop = false;
            for (int j = 0; j < 3; j++){
                if (solutions[i - j].cost() < solutions[i - j - 1].cost()){
                    tries += 1;
                    stop = true;
                }
            }
            if (stop){
                
                if (tries > max_tries){
                    break;
                }
                else{
                    std::cout << "Restarting try " << tries << std::endl;
                    
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

        // if (false){
        if (destroy_dis(gen) <= 3) {
        // if (last_neighbourhood_size < 1) {
            // std::cout << "Random Destroying" << std::endl;
            RandomDestroyer destroyer = RandomDestroyer(this->_instance);
            
            // std::cout << "Solving" << std::endl;
            destroyer.solve(this->_solution);
            // std::cout << "Solved" << std::endl;
            this->_solution = destroyer.get_solution();
            
            std::cout << "Destroyed cost: " << this->_solution.cost() << std::endl;
            std::cout << "T: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;
        }

        Relocate relocate = Relocate(this->_instance);
        relocate.solve(this->_solution);
        this->_solution = relocate.get_solution();

        std::cout << "Relocated cost: " << this->_solution.cost() << std::endl;
        // time elapsed
        std::cout << "T: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

        // std::cout << "Swap" << std::endl;
        Swap swap = Swap(this->_instance);
        // std::cout << "Set Swap Sol" << std::endl;
        swap.set_solution(this->_solution);
        // std::cout << "Exploring" << std::endl;

        for (int j = 0; j < neighbourhood_count; j++) {

            std::vector<std::tuple<int, int, int, int, double>> tabu_neighbours = swap.neighbourhood();

            if (tabu_neighbours.size() == 0) {
                // std::cout << "No neighbours" << std::endl;
                break;
            }
            // std::cout << "Sorting" << std::endl;
            // sort tabu_neighbours by cost
            std::sort(tabu_neighbours.begin(), tabu_neighbours.end(), [](const std::tuple<int, int, int, int, double> &a, const std::tuple<int, int, int, int, double> &b) {
                return std::get<4>(a) < std::get<4>(b);
            });

            // std::uniform_int_distribution<> tabu_dis(0, std::min(10, (int)tabu_neighbours.size() - 1));

            int random_index = 0;//tabu_dis(gen);

            // std::cout << "Random Neighbour: " << random_index << std::endl;

            std::tuple<int, int, int, int, double> best_swap = tabu_neighbours[random_index];

            last_neighbourhood_size = tabu_neighbours.size();

            // if error is less than 40% of the best solution, continue
            if (std::get<4>(best_swap) - this->_solution.cost() > 0.4 * this->_solution.cost()) {
                // std::cout << "Best Swap: " << std::get<4>(best_swap) << std::endl;
                //iter_count--;
                break;
            }

            int v1 = std::get<0>(best_swap);
            int v2 = std::get<1>(best_swap);
            int d1 = std::get<2>(best_swap);
            int d2 = std::get<3>(best_swap);

            // std::cout << "Swapping " << v1 << " " << v2 << " " << d1 << " " << d2 << std::endl;

            swap.do_swap(v1, v2, d1, d2);
            swap.get_solution().recalc_cost();

            // double best_cost = std::get<4>(best_swap);

            // std::cout << "Swapped! j: " << j << " cost: " << swap.get_solution().cost() << std::endl;
            // time elapsed
            // std::cout << "T: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;
        }
        std::cout << "Meta i: " << i << " cost: " << swap.get_solution().cost() << std::endl;
        std::cout << "T: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;
        
        this->_solution = swap.get_solution();
        this->_solution.recalc_cost();

        // std::cout << "New Solution Cost: " << this->_solution.cost() << std::endl;

        solutions.push_back(this->_solution.copy());
    }

    std::sort(solutions.begin(), solutions.end(), [](const GapSolution &a, const GapSolution &b) {
        return a < b;
    });

    this->_solution = solutions[0];


    //end timer
    auto end = std::chrono::high_resolution_clock::now();
    this->_solution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    this->_solution.set_time(this->_solution_time);
}
