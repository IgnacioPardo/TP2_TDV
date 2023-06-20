#include "relocate.h"
#include "greedy_mincost.h"
#include "binpacking.h"
#include "swap.h"
#include <random>

Relocate::~Relocate() {}

void Relocate::solve()
{

    auto start = std::chrono::steady_clock::now();

    BinPacking binpacking(this->_instance);
    binpacking.solve();
    GapSolution solution = binpacking.get_solution();

    this->_solution = solution;
    for (int i = 0; i < 10; i++)
    {
        this->perform_relocation(1000);
    }
}

void Relocate::perform_relocation(int tries = 10)
{
    double partial_cost = this->_solution.cost();

    while (partial_cost == this->_solution.cost() && tries > 0)
    {

        int v;
        int d = -1;

        while (d == -1)
        {

            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> uni(0, this->_solution.n());

            // Obtiene un vendedor al azar
            v = uni(rng) % this->_solution.n();

            // Obtiene el deposito asignado al vendedor
            d = this->_solution.deposito_asignado_al_vendedor(v);
        }

        // std::cout << "Vendedor: " << v << ", Deposito: " << d << std::endl;

        // Recorre todos los depositos
        for (int i = 0; i < this->_solution.m(); i++)
        {
            // Si el deposito tiene espacio para el vendedor
            if (this->get_capacidad_deposito(i) > this->_instance.demanda(i, v))
            {

                // std::cout << "Vendedor: " << v << ", Deposito: " << d << ", Nuevo deposito: " << i << std::endl;
                // std::cout << this->get_capacidad_deposito(i) << " > " << this->_instance.demanda(i, v) << std::endl;

                // Calcula el nuevo costo de integrar el vendedor al deposito y sacarlo del deposito anterior
                double new_cost = partial_cost + this->_instance.cost(i, v) - this->_instance.cost(d, v);

                // Si el nuevo costo es menor al costo parcial
                if (new_cost < partial_cost)
                {
                    // Actualiza el costo parcial
                    partial_cost = new_cost;

                    // Desasigna el deposito del vendedor
                    this->_solution.desasignar_deposito_de_vendedor(d, v);

                    // Asigna el deposito nuevo al vendedor
                    this->_solution.asignar_deposito_a_vendedor(i, v);

                    //this->_solution = solution;
                    this->_solution.set_cost(partial_cost);
                }
            }
        }
        tries--;
    }
}
