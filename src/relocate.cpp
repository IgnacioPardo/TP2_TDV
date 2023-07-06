#include "relocate.h"
#include "greedy_mincost.h"
#include "binpacking.h"
#include "swap.h"
#include <random>

std::string time_format_(double time)
{
    int hours = time / 3600;
    int minutes = (time - hours * 3600) / 60;
    int seconds = time - hours * 3600 - minutes * 60;

    std::string time_str = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);

    return time_str;
}

Relocate::~Relocate() {}

void Relocate::solve(GapSolution solution){
    auto start = std::chrono::steady_clock::now();

    this->_solution = solution;
    this->_solution_time = solution.time();

    this->local_search();

    auto end = std::chrono::steady_clock::now();
    this->_solution_time += std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

void Relocate::perform_relocation(int tries = 1000){
    
    double partial_cost = this->_solution.cost();

    int v_aux;
    int it_sin_mejora = 0;
    
    while (it_sin_mejora < tries){

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

                    // Resetea la cantidad de iteraciones sin mejora
                    it_sin_mejora = 0;
                }
            }
        }
        v_aux = v;
        it_sin_mejora++;
    }
}

void Relocate::local_search(){

    double prev_cost = 0;

    while (this->_solution.cost() < prev_cost || prev_cost == 0)
    {
        
        std::vector<std::tuple<GapSolution, int>> neighbours_w_cost = std::vector<std::tuple<GapSolution, int>>();
        
        int max_cant = this->_instance.n() * this->_instance.m();
        int progress = 0;
        // timer
        auto start = std::chrono::steady_clock::now();

        // O(n*m)
        for (int v = 0; v < this->_solution.n(); v++){
            for (int d = 0; d < this->_solution.m(); d++){
                progress++;

                // erase line
                std::cout << "\033[A\033[2K";

                // print progress with timer and ETA
                std::cout << "Progress: " << progress << "/" << max_cant << " ";
                std::cout << time_format_(std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count() / 1000);
                std::cout << "/" << time_format_(std::chrono::duration<double, std::milli>((std::chrono::steady_clock::now() - start) / progress * (max_cant - progress)).count() / 1000);
                std::cout << std::endl;

                if (this->_solution.deposito_asignado_al_vendedor(v) == d)
                    continue;

                GapSolution neighbour = this->single_relocation(v, d, this->_solution);

                if (neighbour.cost() < this->_solution.cost())
                    neighbours_w_cost.push_back(std::make_tuple(neighbour, neighbour.cost()));
            }
        }

        if (neighbours_w_cost.size() > 0)
        {
            std::sort(neighbours_w_cost.begin(), neighbours_w_cost.end(), [](const std::tuple<GapSolution, int> &a, const std::tuple<GapSolution, int> &b) {
                return std::get<1>(a) < std::get<1>(b);
            });

            // Cantidad de vecinos

            std::cout << "# Neighbours: " << neighbours_w_cost.size() << std::endl << std::endl;

            prev_cost = this->_solution.cost();
            this->_solution = std::get<0>(neighbours_w_cost[0]);
        }
        else
        {
            break;
        }
        
    }
}

GapSolution Relocate::single_relocation(int v, int d, GapSolution sol){

    GapSolution new_sol = sol.copy();

    // Si hay capacidad en el deposito para el vendedor lo reubico
    if (this->get_capacidad_deposito(d) > this->_instance.demanda(d, v)){
        new_sol.desasignar_deposito_de_vendedor(new_sol.deposito_asignado_al_vendedor(v), v);
        new_sol.asignar_deposito_a_vendedor(d, v);
    }
    return new_sol;
}
