#include "greedy_mincost.h"

GreedyMinCost::~GreedyMinCost() {}

std::string GreedyMinCost::get_name() const {
    return "Greedy";
}

void GreedyMinCost::solve(){
    /*
    *   Resuelve el problema de asignacion de vendedores a depositos de manera greedy.
    *   Para cada vendedor, se asigna al deposito que tenga menor costo y que cumpla con la demanda.
    */

    auto start = std::chrono::steady_clock::now();

    this->_status = 1;
    
    for (int j = 0; j < this->_instance.n(); j++) {
        int deposito = this->get_mejor_deposito(j);
        
        if (deposito != -1) {
            this->_solution.asignar_deposito_a_vendedor(deposito, j);
        }
    }
    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

int GreedyMinCost::get_mejor_deposito(int j) {
    /*
    *   Devuelve el deposito que tenga menor costo y que cumpla con la demanda del vendedor j.
    *   Si no hay deposito que cumpla con la demanda del vendedor j, devuelve -1.
    */

    int mejor_deposito = -1;
    int mejor_costo = -1;

    // Get primer deposito que cumpla con:
    // Capacidad del deposito i - capacidad de los vendedores asignados a i >= demanda del vendedor j al deposito i

    for (int deposito = 0; deposito < this->_instance.m(); deposito++) {
        double capacidad_deposito = this->get_capacidad_deposito(deposito);
        double demanda = this->_instance.demanda(deposito, j);
        if (capacidad_deposito >= demanda) {
            mejor_deposito = deposito;
            mejor_costo = this->_instance.cost(deposito, j);
            break;
        }
    }

    // Encontrar el deposito cuya demanda sea menor y que cumpla con:
    // Capacidad del deposito i - capacidad de los vendedores asignados a i >= demanda del vendedor j al deposito i

    for (int deposito = 0; deposito < this->_instance.m(); deposito++) {
        double capacidad_deposito = this->get_capacidad_deposito(deposito);
        double demanda = this->_instance.demanda(deposito, j);

        if (capacidad_deposito >= demanda && this->_instance.cost(deposito, j) < mejor_costo) {
            mejor_deposito = deposito;
            mejor_costo = this->_instance.cost(deposito, j);
        }
    }

    return mejor_deposito;
}
