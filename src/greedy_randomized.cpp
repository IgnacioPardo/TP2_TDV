#include "greedy_randomized.h"

#include <algorithm>
#include <random>

GreedyRandomized::~GreedyRandomized() {}

std::string GreedyRandomized::get_name() const {
    return "GreedyRandomized";
}

void GreedyRandomized::solve() {
    /*
    *   Resuelve el problema de asignacion de vendedores a depositos de manera greedy.
    *   Para cada vendedor, se asigna al deposito que tenga menor costo y que cumpla con la demanda.
    */

    auto start = std::chrono::steady_clock::now();

    this->_status = 1;

    // Ordenar los vendedores de manera aleatoria

    std::vector<int> vendedores(this->_instance.n());
    std::iota(vendedores.begin(), vendedores.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(vendedores.begin(), vendedores.end(), g);

    for (int j : vendedores)
    {
        int deposito = this->get_mejor_deposito(j);

        if (deposito != -1)
        {
            this->_solution.asignar_deposito_a_vendedor(deposito, j);
        }
    }
    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}
