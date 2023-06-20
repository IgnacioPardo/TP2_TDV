#include "greedy_mincost.h"

GreedyMinCost::~GreedyMinCost() {}

void GreedyMinCost::solve(){

    auto start = std::chrono::steady_clock::now();

    this->_status = 1;
    
    for (int j = 0; j < this->_instance.n(); j++) {
        int deposito = this->get_mejor_deposito(j);
        
        //std::cout << "vendedor " << j << " deposito " << deposito << std::endl;

        if (deposito == -1) {
            double demanda_maxima_vj = 0;

            for (int i = 0; i < this->_instance.m(); i++){
                double c_dem = this->_instance.demanda(i, j);
                if (c_dem > demanda_maxima_vj){
                    demanda_maxima_vj = c_dem;
                }
            }
            this->_cost += 3 * demanda_maxima_vj;
        }
        else{
            this->_solution.asignar_deposito_a_vendedor(deposito, j);
            this->_cost += this->_instance.cost(deposito, j);
        }
    }

    this->_solution.set_cost(this->_cost);
    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

int GreedyMinCost::get_mejor_deposito(int j) {
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
