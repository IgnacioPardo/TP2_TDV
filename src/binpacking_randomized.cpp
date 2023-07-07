#include "binpacking_randomized.h"

#include <algorithm>
#include <random>

BinPackingRandomized::~BinPackingRandomized() {}

std::string BinPackingRandomized::get_name() const {
    return "BinPackingRandomized";
}

void BinPackingRandomized::solve()
{
    /*
     *   A cada vendedor j le asigno el deposito i cuya capacidad al restarle la demanda del vendedor j sea la menor
     */

    auto start = std::chrono::steady_clock::now();

    // Por cada vendedor j
    // Busco el deposito i cuya capacidad al restarle la demanda del vendedor j sea la menor

    std::vector<int> vendedores(this->_instance.n());
    std::iota(vendedores.begin(), vendedores.end(), 0);

    auto rng = std::default_random_engine{};
    std::shuffle(vendedores.begin(), vendedores.end(), rng);

    for (int j : vendedores)
    {
        int deposito = this->get_mejor_deposito(j);

        // std::cout << "vendedor " << j << " deposito " << deposito << std::endl;

        this->_solution.asignar_deposito_a_vendedor(deposito, j);
    }

    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}
