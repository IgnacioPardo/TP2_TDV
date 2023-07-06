#include "meta.h"
#include <iostream>
#include <vector>
#include <random>

#include "gap_solution.h"
#include "gap_instance.h"

#include "random_destroyer.h"
#include "greedy_mincost.h"
#include "swap.h"

Meta::~Meta() {}

std::string Meta::get_name() const {
    return "Meta";
}

void Meta::solve() {
    /*
    *  Tabú Swap with Random Destruction
    */

    //start timer
    auto start = std::chrono::high_resolution_clock::now();

    int max_iter = 1000;
    std::random_device rd;
    std::mt19937 gen(rd());

    // std::cout << "Greedy init" << std::endl;

    GreedyMinCost greedy = GreedyMinCost(this->_instance);
    // std::cout << "Greedy Solve" << std::endl;
    greedy.solve();
    this->_solution = greedy.get_solution();
    // std::cout << "Greedy Solved" << std::endl;

    // std::cout << "Touring" << std::endl;
    for (int i = 0; i < max_iter; i++){

        std::uniform_int_distribution<> destroy_dis(1, 10);

        if (destroy_dis(gen) <= 5) {
            // std::cout << "Random Destroying" << std::endl;

            RandomDestroyer destroyer = RandomDestroyer(this->_instance);
            // std::cout << "Solving" << std::endl;
            destroyer.solve(this->_solution);
            // std::cout << "Solved" << std::endl;
            this->_solution = destroyer.get_solution();
        }

        // std::cout << "Swap" << std::endl;
        Swap swap = Swap(this->_instance);
        // std::cout << "Set Swap Sol" << std::endl;
        swap.set_solution(this->_solution);
        // std::cout << "Exploring" << std::endl;
        std::vector<std::tuple<int, int, int, int, double>> tabu_neighbours = swap.neighbourhood();

        if (tabu_neighbours.size() == 0) {
            // std::cout << "No neighbours" << std::endl;
            continue;
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

        // if error is less than 40% of the best solution, continue
        if (std::get<4>(best_swap) - this->_solution.cost() > 0.4 * this->_solution.cost()) {
            // std::cout << "Best Swap: " << std::get<4>(best_swap) << std::endl;
            max_iter--;
            continue;
        }

        int v1 = std::get<0>(best_swap);
        int v2 = std::get<1>(best_swap);
        int d1 = std::get<2>(best_swap);
        int d2 = std::get<3>(best_swap);

        // std::cout << "Swapping " << v1 << " " << v2 << " " << d1 << " " << d2 << std::endl;

        swap.do_swap(v1, v2, d1, d2);

        // std::cout << "Swapped!" << std::endl;

        double best_cost = std::get<4>(best_swap);
        
        this->_solution = swap.get_solution();
        this->_solution.recalc_cost();

        // std::cout << "New Solution Cost: " << this->_solution.cost() << std::endl;
    }

    //end timer
    auto end = std::chrono::high_resolution_clock::now();
    this->_solution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    this->_solution.set_time(this->_solution_time);
}
