#include "swap.h"
#include "greedy_mincost.h"
#include <iostream>
#include <random>

Swap::~Swap() {}

void Swap::solve() {
    auto start = std::chrono::steady_clock::now();

    GreedyMinCost greedy(this->_instance);
    greedy.solve();

    GapSolution solution = greedy.get_solution();

    int new_cost;
    
    for (int i = 0; i < 10; i++){
        new_cost = this->perform_swap(solution, 10);
    }
    
    this->_solution = solution;
    this->_solution.set_cost(new_cost);

    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

int Swap::perform_swap(GapSolution &solution, int tries = 10){
    GapSolution partial_solution = solution;

    double partial_cost = solution.cost();

    while (partial_cost == solution.cost() && tries > 0){

        int v_i;
        int v_j;
        int d_j = -1;
        int d_i = -1;
        
        
        while (d_i == -1 || d_j == -1){

            std::random_device rd;     
            std::mt19937 rng(rd());   
            std::uniform_int_distribution<int> uni(0,solution.n()); 

            // Tomamos dos vendedores al azar
            v_i = uni(rng) % solution.n();
            v_j = uni(rng) % solution.n();

            // Obtenemos los depositos asignados a cada vendedor
            d_j = solution.deposito_asignado_al_vendedor(v_j);
            d_i = solution.deposito_asignado_al_vendedor(v_i);
        }
    
        // Calculamos el espacio disponible en cada deposito para los vendedores en el caso de intercambiarlos
        int room_in_deposit_i = this->get_capacidad_deposito(d_i) + this->_instance.demanda(d_i, v_i);
        int room_in_deposit_j = this->get_capacidad_deposito(d_j) + this->_instance.demanda(d_j, v_j);

        // Si ambos depositos tienen espacio para los vendedores
        if (room_in_deposit_i >= this->_instance.demanda(d_i, v_j) && room_in_deposit_j >= this->_instance.demanda(d_j, v_i)) {
            
            // Calculamos el nuevo costo de intercambiar los vendedores
            double new_cost = partial_cost + this->_instance.cost(d_i, v_j) + this->_instance.cost(d_j, v_i) - this->_instance.cost(d_i, v_i) - this->_instance.cost(d_j, v_j);

            // Si el nuevo costo es menor al costo parcial
            if (new_cost < partial_cost) {
                
                // Actualizamos el costo parcial
                partial_cost = new_cost;

                // Intercambiamos los depositos de los vendedores
                solution.desasignar_deposito_de_vendedor(d_i, v_i);
                solution.desasignar_deposito_de_vendedor(d_j, v_j);

                solution.asignar_deposito_a_vendedor(d_i, v_j);
                solution.asignar_deposito_a_vendedor(d_j, v_i);
            } 
        }
        tries--;
    }
    return partial_cost;
}

