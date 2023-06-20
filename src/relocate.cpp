#include "relocate.h"
#include "greedy_mincost.h"
#include <random>

Relocate::~Relocate() {}

void Relocate::solve() {
    
    auto start = std::chrono::steady_clock::now();

    GreedyMinCost greedy(this->_instance);
    
    greedy.solve();

    GapSolution solution = greedy.get_solution();

    int new_cost;
    for(int i = 0; i < 10; i++){
        new_cost = this->perform_relocation(solution, 1000);
    }

    this->_solution = solution;
    this->_solution.set_cost(new_cost);
}

int Relocate::perform_relocation(GapSolution &solution, int tries=10){
    double partial_cost = solution.cost();

    while (partial_cost == solution.cost() && tries > 0) {
 
        int v;
        int d = -1;
        
        while (d == -1) {
            
            std::random_device rd;     
            std::mt19937 rng(rd());   
            std::uniform_int_distribution<int> uni(0,solution.n()); 

            //Obtiene un vendedor al azar
            v = uni(rng) % solution.n();          

            //Obtiene el deposito asignado al vendedor
            d = solution.deposito_asignado_al_vendedor(v);
        }

        // std::cout << "Vendedor: " << v << ", Deposito: " << d << std::endl;
        
        //Recorre todos los depositos
        for(int i = 0; i < solution.m(); i++){
            // Si el deposito tiene espacio para el vendedor
            if(this->get_capacidad_deposito(i) > this->_instance.demanda(i, v)){
                // Calcula el nuevo costo de integrar el vendedor al deposito y sacarlo del deposito anterior
                double new_cost = partial_cost + this->_instance.cost(i, v) - this->_instance.cost(d, v);

                // Si el nuevo costo es menor al costo parcial
                if (new_cost < partial_cost) {
                    
                    // Actualiza el costo parcial
                    partial_cost = new_cost;
                    
                    // Desasigna el deposito del vendedor
                    solution.desasignar_deposito_de_vendedor(d, v);
                    
                    // Asigna el deposito nuevo al vendedor
                    solution.asignar_deposito_a_vendedor(i, v);
                }
            }
        }
        tries--;
    }
    
    return partial_cost;
}