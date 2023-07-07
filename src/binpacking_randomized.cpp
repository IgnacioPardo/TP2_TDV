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
    *   Para cada vendedor j en orden aleatorio
    *   Le asigno el deposito i cuya capacidad al restarle la demanda del vendedor j sea la menor
    */

    auto start = std::chrono::steady_clock::now();

    // Por cada vendedor j
    // Busco el deposito i cuya capacidad al restarle la demanda del vendedor j sea la menor

    // Randomizar orden de vendedores 0-n
    std::vector<int> vendedores(this->_instance.n());
    std::iota(vendedores.begin(), vendedores.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(vendedores.begin(), vendedores.end(), g);

    for (int j : vendedores)
    {
        int deposito = this->get_mejor_deposito(j);

        if (deposito != -1)
            this->_solution.asignar_deposito_a_vendedor(deposito, j);
    }

    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}
