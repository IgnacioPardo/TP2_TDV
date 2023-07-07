#include "swap.h"
#include <iostream>

Swap::~Swap() {}

std::string Swap::get_name() const {
    return "Swap";
}

void Swap::solve(GapSolution solution) {
    /*
    *  Aplica la heuristica de busqueda local Swap a la solucion pasada por parametro.
    *  La solucion devuelta es la mejor solucion encontrada.
    */
    auto start = std::chrono::steady_clock::now();
    this->_solution = solution;
    this->_solution_time = solution.time();

    this->local_search();

    auto end = std::chrono::steady_clock::now();
    this->_solution_time += std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

std::vector<std::tuple<int, int, int, int, double>> Swap::neighbourhood(){
    /*
    *   Genera los vecinos de la solución actual
    *   Los vecinos son todas aquellas soluciones que puedo obtener de intercambiar los depósitos asignados entre dos vendedores.
    */

    std::vector<std::tuple<int, int, int, int, double>> swaps_w_cost = std::vector<std::tuple<int, int, int, int, double>>();

    // O(n * (n^2 - n) / 2)
    for (int v1 = 0; v1 < this->_instance.n(); v1++)
    {
        for (int v2 = v1 + 1; v2 < this->_instance.n(); v2++)
        {

            if (v1 == v2)
                continue;

            // O(1)
            int d1 = this->_solution.deposito_asignado_al_vendedor(v1);
            int d2 = this->_solution.deposito_asignado_al_vendedor(v2);

            if (d1 == d2)
                continue;
            if (d1 == -1 && d2 == -1)
                continue;

            std::tuple<bool, double> posible_swap = this->single_swap(v1, v2, d1, d2);

            if (std::get<0>(posible_swap))
            {
                if (std::get<1>(posible_swap) < this->_solution.cost())
                {
                    // std::cout << "Swap: " << v1 << " " << v2 << " " << d1 << " " << d2 << " " << std::get<1>(posible_swap) << std::endl;
                    swaps_w_cost.push_back(std::make_tuple(v1, v2, d1, d2, std::get<1>(posible_swap)));
                }
            }
        }
    }

    return swaps_w_cost;
}

void Swap::local_search(){
    /*
    *   Genera el vecindario de la solución actual y se queda con el mejor vecino.
    *   Repite el proceso hasta que no haya un vecino mejor que la solución actual.
    */
    // Costo inicial

    double prev_cost = 0;
    
    // Best Improvement
    while (this->_solution.cost() < prev_cost || prev_cost == 0){
        
        std::vector<std::tuple<int, int, int, int, double>> neighbours = this->neighbourhood();
        
        if (neighbours.size() > 0)
        {   

            // Ordeno los vecinos por costo
            std::sort(neighbours.begin(), neighbours.end(), [](std::tuple<int, int, int, int, double> a, std::tuple<int, int, int, int, double> b) {
                return std::get<4>(a) < std::get<4>(b);
            });


            prev_cost = this->_solution.cost();

            std::tuple<int, int, int, int, double> best_swap = neighbours[0];

            int v1 = std::get<0>(best_swap);
            int v2 = std::get<1>(best_swap);
            int d1 = std::get<2>(best_swap);
            int d2 = std::get<3>(best_swap);

            this->do_swap(v1, v2, d1, d2);
            
            double best_cost = std::get<4>(best_swap);
            this->_solution.set_cost(best_cost);

        }
        else
        {
            break;
        }
    }
}

void Swap::do_swap(int v1, int v2, int d1, int d2){
    /*
    *   Realiza el intercambio de depósitos entre dos vendedores.
    */
    if (d1 == -1 && d2 == -1){
        return;
    }
    else if (d1 == -1){
        this->_solution.desasignar_deposito_de_vendedor(d2, v2);
        this->_solution.asignar_deposito_a_vendedor(d2, v1);
        return;
    }
    else if (d2 == -1){
        this->_solution.desasignar_deposito_de_vendedor(d1, v1);
        this->_solution.asignar_deposito_a_vendedor(d1, v2);
        return;
    }
    else{
        this->_solution.desasignar_deposito_de_vendedor(d1, v1);
        this->_solution.desasignar_deposito_de_vendedor(d2, v2);
        this->_solution.asignar_deposito_a_vendedor(d1, v2);
        this->_solution.asignar_deposito_a_vendedor(d2, v1);
    }
}

std::tuple<bool, double> Swap::single_swap(int v1, int v2, int d1, int d2){
    /*
    *   Realiza el intercambio de depósitos entre dos vendedores sin modificar la solución actual.
    *   Devuelve un booleano indicando si el intercambio es posible y el costo de la solución resultante.
    */
    
    if (this->_solution.deposito_asignado_al_vendedor(v1) == -1){ // O(1)
        return this->swap_over_unasigned(v2, v1, d2);
    }
    else if (this->_solution.deposito_asignado_al_vendedor(v2) == -1){ // O(1)
        return this->swap_over_unasigned(v1, v2, d1);
    }

    // O(1)
    if (this->get_capacidad_deposito(d1) + this->_instance.demanda(d1, v1) > this->_instance.demanda(d1, v2)
     && this->get_capacidad_deposito(d2) + this->_instance.demanda(d2, v2) > this->_instance.demanda(d2, v1)){
        double new_cost = this->_solution.cost() - this->_instance.cost(d1, v1) - this->_instance.cost(d2, v2) + this->_instance.cost(d1, v2) + this->_instance.cost(d2, v1);
        
        return std::make_tuple(true, new_cost);
    }

    return std::make_tuple(false, 0);
}

std::tuple<bool, double> Swap::swap_over_unasigned(int va, int vu, int da){
    /*
    *   Realiza el intercambio de depósitos entre un vendedor asignado y uno no asignado sin modificar la solución actual.
    *   Devuelve un booleano indicando si el intercambio es posible y el costo de la solución resultante.
    */

    if (this->get_capacidad_deposito(da) + this->_instance.demanda(da, va) > this->_instance.demanda(da, vu))
    {
        return std::make_tuple(true, this->_solution.cost() - this->_instance.cost(da, va) + this->_instance.cost(da, vu));
    }

    return std::make_tuple(false, 0);
}

void Swap::set_solution(GapSolution solution){
    // Actualiza la solución actual.
    this->_solution = solution;
}
