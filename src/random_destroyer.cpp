#include "random_destroyer.h"
#include <iostream>
#include <vector>
#include <random>


RandomDestroyer::~RandomDestroyer() {}

std::string RandomDestroyer::get_name() const {
    return "RandomDestroyer";
}

void RandomDestroyer::solve(GapSolution solution){
    /*
    *   "Destruye" la solucion pasada por parametro
    *   Reasigna clientes aleatorios a depositos aleatorios una cantidad aleatoria de veces
    */
    this->_solution = solution;
    
    // timer
    auto start = std::chrono::high_resolution_clock::now();

    this->perform_destruction();

    this->_solution.recalc_cost();

    // timer
    auto end = std::chrono::high_resolution_clock::now();
    this->_solution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    this->_solution.set_time(this->_solution_time);
}

void RandomDestroyer::perform_destruction(){
    /*
    * Reasigna un cliente aleatorio a un deposito aleatorio
    * N veces aleatorias
    */

    // random ammount of times
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, this->_instance.n());

    int ammount = dis(gen);

    for (int i = 0; i < ammount; i++) {
        // Cliente aleatorio
        std::uniform_int_distribution<> dis2(0, this->_instance.n() - 1);
        int v = dis2(gen);

        // Deposito aleatorio
        std::uniform_int_distribution<> dis3(-1, this->_instance.m() - 1);
        int d = dis3(gen);

        int prev_d = this->_solution.deposito_asignado_al_vendedor(v);
        if (d == -1) {
            if (prev_d != -1 && this->_is_unassigner)
                this->_solution.desasignar_deposito_de_vendedor(prev_d, v);
            continue;
        }

        // Si el cliente no esta asignado al deposito
        if (prev_d != d) {
            // Si el deposito tiene capacidad para el cliente
            if (this->_solution.capacidad_actual_deposito(d) >= this->_instance.demanda(d, v)) {
                // Si el cliente esta asignado a otro deposito
                if (prev_d != -1) {
                    // Desasignar el cliente del deposito
                    this->_solution.desasignar_deposito_de_vendedor(d, v);
                }
                // Asignar el cliente al deposito
                this->_solution.asignar_deposito_a_vendedor(d, v);
            }
        }
    }
}

void RandomDestroyer::set_is_unassigner(bool is_unassigner) {
    this->_is_unassigner = is_unassigner;
}
