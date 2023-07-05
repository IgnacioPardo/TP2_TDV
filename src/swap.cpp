#include "swap.h"
#include "greedy_mincost.h"
#include "binpacking.h"
#include <iostream>
#include <random>

Swap::~Swap() {}

void Swap::solve() {
    auto start = std::chrono::steady_clock::now();

    GreedyMinCost greedy(this->_instance);
    greedy.solve();
    GapSolution solution = greedy.get_solution();

    this->_solution = solution;
    this->_solution_time = solution.time();
    
    this->perform_swap();

    auto end = std::chrono::steady_clock::now();
    this->_solution_time += std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

void Swap::solve(GapSolution solution) {
    auto start = std::chrono::steady_clock::now();

    this->_solution = solution;
    this->_solution_time = solution.time();
    
    this->perform_swap();
    

    auto end = std::chrono::steady_clock::now();
    this->_solution_time += std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

void Swap::perform_swap(){
    GapSolution partial_solution = this->_solution;
    
    double partial_cost = this->_solution.cost();
    
    int it_sin_mejora = 0;

    
    int d_v = -1;
    int d_aux = -1;
    
    for (int v = 0; v < this->_solution.n(); v++){
        for (int v_aux = 0; v_aux < this->_solution.n(); v_aux++){
        
            if (v != v_aux && this->_solution.deposito_asignado_al_vendedor(v) != this->_solution.deposito_asignado_al_vendedor(v_aux)){
                
                // Obtenemos los depósitos asignados a cada vendedor
                d_v = this->_solution.deposito_asignado_al_vendedor(v);
                d_aux = this->_solution.deposito_asignado_al_vendedor(v_aux);


                // El vendedor v tiene depósito asignado y el vendedor v_aux no
                if (d_aux == -1 && d_v != -1) {
                    
                    double room_in_deposit_i = this->get_capacidad_deposito(d_v) + this->_instance.demanda(d_v, v);

                    if (room_in_deposit_i >= this->_instance.demanda(d_v, v_aux)) {

                        // Calculamos el nuevo costo de intercambiar los vendedores
                        double new_cost = partial_cost + this->_instance.cost(d_v, v_aux) - this->_instance.cost(d_v, v) - this->_instance.penalizacion(v_aux) + this->_instance.penalizacion(v);

                        // Si el nuevo costo es menor al costo parcial
                        if (new_cost < partial_cost) {
                            
                            // Actualizamos el costo parcial
                            partial_cost = new_cost;

                            // Desasignamos el depósito del vendedor v_aux y asignamos el depósito al vendedor v
                            this->_solution.desasignar_deposito_de_vendedor(d_v, v);
                            this->_solution.asignar_deposito_a_vendedor(d_v, v_aux);

                            it_sin_mejora = 0; // Reseteo si hubo mejora
                        }
                    }
                }

                // El vendedor v_aux tiene depósito asignado y el vendedor v no
                else if (d_aux != -1 && d_v == -1) {
                    
                    double room_in_deposit_j = this->get_capacidad_deposito(d_aux) + this->_instance.demanda(d_aux, v_aux);

                    if (room_in_deposit_j >= this->_instance.demanda(d_aux, v)) {

                        // Calculamos el nuevo costo de intercambiar los vendedores
                        double new_cost = partial_cost + this->_instance.cost(d_aux, v) - this->_instance.cost(d_aux, v_aux) - this->_instance.penalizacion(v) + this->_instance.penalizacion(v_aux);

                        // Si el nuevo costo es menor al costo parcial
                        if (new_cost < partial_cost) {

                            // Actualizamos el costo parcial
                            partial_cost = new_cost;

                            // Desasignamos el depósito del vendedor i y asignamos el depósito al vendedor j
                            this->_solution.desasignar_deposito_de_vendedor(d_aux, v_aux);
                            this->_solution.asignar_deposito_a_vendedor(d_aux, v);

                            it_sin_mejora = 0; // Reseteo si hubo mejora
                        }
                    }
                }

                // Ambos vendedores tienen depósito asignado
                else {
                    // Calculamos el espacio disponible en cada depósito para los vendedores en el caso de intercambiarlos
                    double room_in_deposit_i = this->get_capacidad_deposito(d_v) + this->_instance.demanda(d_v, v);
                    double room_in_deposit_j = this->get_capacidad_deposito(d_aux) + this->_instance.demanda(d_aux, v_aux);

                    // Si ambos depósitos tienen espacio para los vendedores
                    if (room_in_deposit_i >= this->_instance.demanda(d_v, v_aux) && room_in_deposit_j >= this->_instance.demanda(d_aux, v)) {

                        // Calculamos el nuevo costo de intercambiar los vendedores
                        double new_cost = partial_cost + this->_instance.cost(d_v, v_aux) + this->_instance.cost(d_aux, v) - this->_instance.cost(d_v, v) - this->_instance.cost(d_aux, v_aux);

                        // Si el nuevo costo es menor al costo parcial
                        if (new_cost < partial_cost) {

                            // Actualizamos el costo parcial
                            partial_cost = new_cost;

                            // Intercambiamos los depósitos de los vendedores
                            this->_solution.desasignar_deposito_de_vendedor(d_v, v);
                            this->_solution.desasignar_deposito_de_vendedor(d_aux, v_aux);

                            this->_solution.asignar_deposito_a_vendedor(d_v, v_aux);
                            this->_solution.asignar_deposito_a_vendedor(d_aux, v);
                        }
                    }
                }
            }
        }   
    }

    this->_solution.set_cost(partial_cost);
}
