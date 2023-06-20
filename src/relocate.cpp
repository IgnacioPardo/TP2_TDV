#include "relocate.h"
#include "greedy_mincost.h"
#include <random>

Relocate::~Relocate() {}

void Relocate::solve() {
    
    auto start = std::chrono::steady_clock::now();

    GreedyMinCost greedy(this->_instance);
    greedy.solve();

    GapSolution solution = greedy.get_solution();

    double partial_cost = solution.cost();

    while (partial_cost == solution.cost()) {
 
        int v;
        int d = -1;
        
        while (d == -1) {
            
            std::random_device rd;     
            std::mt19937 rng(rd());   
            std::uniform_int_distribution<int> uni(0,solution.n()); 

            v = uni(rng) % solution.n();                     //Elige un vendedor al azar
            d = solution.deposito_asignado_al_vendedor(v);   //Obtiene el deposito asignado al vendedor
        }
        
        for(int i = 0; i < solution.m(); i++){                  //Recorre todos los depositos
            if(this->get_capacidad_deposito(i) > this->_instance.demanda(i, v)){   // Si el deposito tiene espacio para el vendedor
                double new_cost = partial_cost + this->_instance.cost(i, v) - this->_instance.cost(d, v); // Calcula el nuevo costo de integrar el vendedor al deposito y sacarlo del deposito anterior

                if (new_cost < partial_cost) {  // Si el nuevo costo es menor al costo parcial
                    
                    partial_cost = new_cost;    // Actualiza el costo parcial

                    solution.desasignar_deposito_de_vendedor(d, v); // Desasigna el deposito del vendedor
                    solution.asignar_deposito_a_vendedor(i, v);     // Asigna el deposito nuevo al vendedor
                }
            }
        }
    }

    this->_solution = solution;
}
    