#include "relocate.h"
#include "greedy_mincost.h"
#include "binpacking.h"
#include "swap.h"
#include <random>

Relocate::~Relocate() {}

void Relocate::solve(){
    auto start = std::chrono::steady_clock::now();

    // GreedyMinCost greedy(this->_instance);
    // greedy.solve();
    // GapSolution solution = greedy.get_solution();

    BinPacking binpacking(this->_instance);
    binpacking.solve();
    GapSolution solution = binpacking.get_solution();

    this->_solution = solution;
    this->_solution_time = solution.time();

    for (int i = 0; i < 10; i++){
        this->perform_relocation(1000);
    }

    auto end = std::chrono::steady_clock::now();
    this->_solution_time += std::chrono::duration<double, std::milli>(end - start).count();
    this->_solution.set_time(this->_solution_time);
}

void Relocate::solve(GapSolution solution){
    auto start = std::chrono::steady_clock::now();

    this->_solution = solution;
    this->_solution_time = solution.time();

    for (int i = 0; i < 10; i++){
        this->perform_relocation(10);
    }

    auto end = std::chrono::steady_clock::now();
    this->_solution_time += std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

void Relocate::perform_relocation(int tries = 1000){
    
    double partial_cost = this->_solution.cost();

    int v_aux;
    
    while (partial_cost == this->_solution.cost() && tries > 0){

        int v;
        int d = -1;

        // Si el vendedor no tiene deposito asignado o el vendedor es el mismo que el anterior
        while (d == -1 || v == v_aux){

            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> uni(0, this->_solution.n());

            // Obtiene un vendedor al azar
            v = uni(rng) % this->_solution.n();

            // Obtiene el deposito asignado al vendedor
            d = this->_solution.deposito_asignado_al_vendedor(v);
        }

        // Recorre todos los depositos
        for (int i = 0; i < this->_solution.m(); i++){
            
            // Si el deposito tiene espacio para el vendedor
            if (this->get_capacidad_deposito(i) > this->_instance.demanda(i, v) && i != d){

                int d_aux = this->_solution.deposito_asignado_al_vendedor(v);

                // Calcula el nuevo costo de integrar el vendedor al deposito y sacarlo del deposito anterior
                double new_cost = partial_cost + this->_instance.cost(i, v) - this->_instance.cost(d_aux, v);

                // Si el nuevo costo es menor al costo parcial
                if (new_cost < partial_cost){
                    // Actualiza el costo parcial
                    partial_cost = new_cost;

                    // Desasigna el deposito del vendedor
                    this->_solution.desasignar_deposito_de_vendedor(d_aux, v);

                    // Asigna el deposito nuevo al vendedor
                    this->_solution.asignar_deposito_a_vendedor(i, v);

                    //this->_solution = solution;
                    this->_solution.set_cost(partial_cost);
                }
            }
        }
        v_aux = v;
        tries--;
    }
}
