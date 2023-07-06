#include "swap.h"
#include "greedy_mincost.h"
#include "binpacking.h"
#include <iostream>
#include <random>

std::string time_format(double time)
{
    int hours = time / 3600;
    int minutes = (time - hours * 3600) / 60;
    int seconds = time - hours * 3600 - minutes * 60;

    std::string time_str = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);

    return time_str;
}

Swap::~Swap() {}

void Swap::solve(GapSolution solution) {
    auto start = std::chrono::steady_clock::now();
    this->_solution = solution;
    this->_solution_time = solution.time();

    this->local_search();

    auto end = std::chrono::steady_clock::now();
    this->_solution_time += std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

void Swap::local_search(){
    // Costo inicial

    double prev_cost = 0;
    
    while (this->_solution.cost() < prev_cost || prev_cost == 0){

        
        // std::cout << "New Solution" << std::endl;

        std::vector<std::tuple<int, int, int, int, double>> swaps_w_cost = std::vector<std::tuple<int, int, int, int, double>>();

        // O(n * (n^2 - n) / 2)
        int max_cant = this->_instance.n() * (this->_instance.n() - 1) / 2;
        int progress = 0;

        // timer
        auto start = std::chrono::steady_clock::now();

        for (int v1 = 0; v1 < this->_instance.n(); v1++){
           for (int v2 = v1 + 1; v2 < this->_instance.n(); v2++){
                progress++;

                // std::cout << "\033[A\033[2K";
                // std::cout << "Progress: " << progress << "/" << max_cant << " ";
                // std::cout << time_format(std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count() / 1000);
                // std::cout << "/" << time_format(std::chrono::duration<double, std::milli>((std::chrono::steady_clock::now() - start) / progress * (max_cant - progress)).count() / 1000);
                // std::cout << std::endl;

                if (v1 == v2)
                    continue;
                
                // O(1)
                int d1 = this->_solution.deposito_asignado_al_vendedor(v1);
                int d2 = this->_solution.deposito_asignado_al_vendedor(v2);

                if (d1 == d2)
                    continue;
                if (d1 == -1 && d2 == -1)
                    continue;

                // std::cout << "Checking v1: " << v1 << " v2: " << v2 << " d1: " << d1 << " d2: " << d2 << std::endl;

                std::tuple<bool, double> posible_swap = this->single_swap(v1, v2, d1, d2);

                if (std::get<0>(posible_swap)){
                    // std::cout << "Current cost: " << this->_solution.cost() << std::endl;
                    // std::cout << "Swap Cost: " << std::get<1>(posible_swap) << std::endl;
                    if (std::get<1>(posible_swap) < this->_solution.cost()){
                        // std::cout << "Swap v1: " << v1 << " v2: " << v2 << " d1: " << d1 << " d2: " << d2 << std::endl;
                        // std::cout << "Cost: " << std::get<1>(posible_swap) << std::endl;
                        swaps_w_cost.push_back(std::make_tuple(v1, v2, d1, d2, std::get<1>(posible_swap)));
                        // std::cout << "added" << std::endl;
                    }
                }
            }
        }
        
        // std::cout << "# Neighbours: " << swaps_w_cost.size() << std::endl;
        
        // erase progress
        // std::cout << "\033[A\033[2K";

        if (swaps_w_cost.size() > 0)
        {
            
            // sort neighbours by cost
            std::sort(swaps_w_cost.begin(), swaps_w_cost.end(), [](std::tuple<int, int, int, int, double> a, std::tuple<int, int, int, int, double> b) {
                return std::get<4>(a) < std::get<4>(b);
            });

            // std::cout << "Sorted" << std::endl;

            prev_cost = this->_solution.cost();
            // std::cout << "Prev Cost: " << prev_cost << std::endl;
            // perform swap

            std::tuple<int, int, int, int, double> best_swap = swaps_w_cost[0];

            int v1 = std::get<0>(best_swap);
            int v2 = std::get<1>(best_swap);
            int d1 = std::get<2>(best_swap);
            int d2 = std::get<3>(best_swap);

            // std::cout << "Best Swap v1: " << v1 << " v2: " << v2 << " d1: " << d1 << " d2: " << d2 << std::endl;
            // std::cout << "Cost: " << std::get<4>(best_swap) << std::endl;

            this->do_swap(v1, v2, d1, d2);
            
            // this->_solution.set_cost(std::get<4>(best_swap));
            this->_solution.recalc_cost();
        
        }
        else
        {
            break;
        }
    }
}

void Swap::do_swap(int v1, int v2, int d1, int d2){

    // std::cout << "Doing swap" << std::endl;

    if (d1 == -1){
        // std::cout << "d1 == -1" << std::endl;
        this->_solution.desasignar_deposito_de_vendedor(d2, v2);
        this->_solution.asignar_deposito_a_vendedor(d2, v1);
        return;
    }
    else if (d2 == -1){
        // std::cout << "d2 == -1" << std::endl;
        this->_solution.desasignar_deposito_de_vendedor(d1, v1);
        this->_solution.asignar_deposito_a_vendedor(d1, v2);
        return;
    }

    // std::cout << "Symmetric" << std::endl;

    this->_solution.desasignar_deposito_de_vendedor(d1, v1);
    this->_solution.desasignar_deposito_de_vendedor(d2, v2);
    this->_solution.asignar_deposito_a_vendedor(d1, v2);
    this->_solution.asignar_deposito_a_vendedor(d2, v1);
}

std::tuple<bool, double> Swap::single_swap(int v1, int v2, int d1, int d2){
    
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
        
        
        if (this->_solution.cost() > new_cost)
        {
            // std::cout << "Better Cost: " << new_cost << std::endl;
            return std::make_tuple(true, new_cost);
        }
    }

    return std::make_tuple(false, 0);
}

std::tuple<bool, double> Swap::swap_over_unasigned(int va, int vu, int da){

    if (this->get_capacidad_deposito(da) + this->_instance.demanda(da, va) > this->_instance.demanda(da, vu))
    {
        return std::make_tuple(true, this->_solution.cost() - this->_instance.cost(da, vu) + this->_instance.cost(da, va));
    }

    return std::make_tuple(false, 0);
}

void Swap::perform_swap()
{
    GapSolution neighbour = this->_solution.copy();

    double partial_cost = this->_solution.cost();

    int d_v = -1;
    int d_aux = -1;

    for (int v = 0; v < neighbour.n(); v++)
    {
        for (int v_aux = 0; v_aux < neighbour.n(); v_aux++)
        {

            if (v != v_aux && neighbour.deposito_asignado_al_vendedor(v) != neighbour.deposito_asignado_al_vendedor(v_aux))
            {

                // Obtenemos los depósitos asignados a cada vendedor
                d_v = neighbour.deposito_asignado_al_vendedor(v);
                d_aux = neighbour.deposito_asignado_al_vendedor(v_aux);

                // El vendedor v tiene depósito asignado y el vendedor v_aux no
                if (d_aux == -1 && d_v != -1)
                {

                    double room_in_deposit_i = this->get_capacidad_deposito(d_v) + this->_instance.demanda(d_v, v);

                    if (room_in_deposit_i >= this->_instance.demanda(d_v, v_aux))
                    {

                        // Calculamos el nuevo costo de intercambiar los vendedores
                        double new_cost = partial_cost + this->_instance.cost(d_v, v_aux) - this->_instance.cost(d_v, v) - this->_instance.penalizacion(v_aux) + this->_instance.penalizacion(v);

                        // Si el nuevo costo es menor al costo parcial
                        if (new_cost < partial_cost)
                        {

                            // Actualizamos el costo parcial
                            partial_cost = new_cost;

                            // Desasignamos el depósito del vendedor v_aux y asignamos el depósito al vendedor v
                            neighbour.desasignar_deposito_de_vendedor(d_v, v);
                            neighbour.asignar_deposito_a_vendedor(d_v, v_aux);

                        }
                    }
                }

                // El vendedor v_aux tiene depósito asignado y el vendedor v no
                else if (d_aux != -1 && d_v == -1)
                {

                    double room_in_deposit_j = this->get_capacidad_deposito(d_aux) + this->_instance.demanda(d_aux, v_aux);

                    if (room_in_deposit_j >= this->_instance.demanda(d_aux, v))
                    {

                        // Calculamos el nuevo costo de intercambiar los vendedores
                        double new_cost = partial_cost + this->_instance.cost(d_aux, v) - this->_instance.cost(d_aux, v_aux) - this->_instance.penalizacion(v) + this->_instance.penalizacion(v_aux);

                        // Si el nuevo costo es menor al costo parcial
                        if (new_cost < partial_cost)
                        {

                            // Actualizamos el costo parcial
                            partial_cost = new_cost;

                            // Desasignamos el depósito del vendedor i y asignamos el depósito al vendedor j
                            neighbour.desasignar_deposito_de_vendedor(d_aux, v_aux);
                            neighbour.asignar_deposito_a_vendedor(d_aux, v);

                        }
                    }
                }

                // Ambos vendedores tienen depósito asignado
                else
                {
                    // Calculamos el espacio disponible en cada depósito para los vendedores en el caso de intercambiarlos
                    double room_in_deposit_i = this->get_capacidad_deposito(d_v) + this->_instance.demanda(d_v, v);
                    double room_in_deposit_j = this->get_capacidad_deposito(d_aux) + this->_instance.demanda(d_aux, v_aux);

                    // Si ambos depósitos tienen espacio para los vendedores
                    if (room_in_deposit_i >= this->_instance.demanda(d_v, v_aux) && room_in_deposit_j >= this->_instance.demanda(d_aux, v))
                    {

                        // Calculamos el nuevo costo de intercambiar los vendedores
                        double new_cost = partial_cost + this->_instance.cost(d_v, v_aux) + this->_instance.cost(d_aux, v) - this->_instance.cost(d_v, v) - this->_instance.cost(d_aux, v_aux);

                        // Si el nuevo costo es menor al costo parcial
                        if (new_cost < partial_cost)
                        {

                            // Actualizamos el costo parcial
                            partial_cost = new_cost;

                            // Intercambiamos los depósitos de los vendedores
                            neighbour.desasignar_deposito_de_vendedor(d_v, v);
                            neighbour.desasignar_deposito_de_vendedor(d_aux, v_aux);

                            neighbour.asignar_deposito_a_vendedor(d_v, v_aux);
                            neighbour.asignar_deposito_a_vendedor(d_aux, v);
                        }
                    }
                }
            }
        }
    }
}
