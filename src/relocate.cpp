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
        
        // std::vector<std::tuple<GapSolution, int>> neighbours_w_cost = std::vector<std::tuple<GapSolution, int>>();

        std::vector<std::tuple<int, int, int>> relocation_w_cost = std::vector<std::tuple<int, int, int>>();
        
        int max_cant = this->_instance.n() * this->_instance.m();
        int progress = 0;
        // timer
        auto start = std::chrono::steady_clock::now();

        // O(n*m)
        for (int v = 0; v < this->_solution.n(); v++){
            for (int d = 0; d < this->_solution.m(); d++){
                progress++;

                // std::cout << "\033[A\033[2K";
                // std::cout << "Progress: " << progress << "/" << max_cant << " ";
                // std::cout << time_format_(std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count() / 1000);
                // std::cout << "/" << time_format_(std::chrono::duration<double, std::milli>((std::chrono::steady_clock::now() - start) / progress * (max_cant - progress)).count() / 1000);
                // std::cout << std::endl;

                if (this->_solution.deposito_asignado_al_vendedor(v) == d)
                    continue;

                std::tuple<bool, double> relocation = this->single_relocation(v, d);

                if (std::get<0>(relocation))
                {
                    if (std::get<1>(relocation) < this->_solution.cost()){
                        // std::cout << "Cost: " << this->_solution.cost() << " -> " << std::get<1>(relocation) << std::endl;
                        relocation_w_cost.push_back(std::make_tuple(v, d, std::get<1>(relocation)));
                    }
                }
            }
        }

        if (relocation_w_cost.size() > 0)
        {
            std::sort(relocation_w_cost.begin(), relocation_w_cost.end(), [](std::tuple<int, int, int> a, std::tuple<int, int, int> b) {
                return std::get<2>(a) < std::get<2>(b);
            });

            // Cantidad de vecinos

            // std::cout << "# Neighbours: " << relocation_w_cost.size() << std::endl;

            prev_cost = this->_solution.cost();
            
            std::tuple<int, int, int> best_relocation = relocation_w_cost[0];

            int v = std::get<0>(best_relocation);
            int d = std::get<1>(best_relocation);
            double best_cost = std::get<2>(best_relocation);

            // std::cout << "Relocating " << v << " to " << d << " with cost " << best_cost << std::endl;

            int prev_d = this->_solution.deposito_asignado_al_vendedor(v);

            // std::cout << "Relocating " << v << " from prev d: " << prev_d << " new d: " << d << std::endl;
            // std::cout << "with costs: " << this->_instance.cost(prev_d, v) << " -> " << this->_instance.cost(d, v) << std::endl;

            this->_solution.desasignar_deposito_de_vendedor(prev_d, v);
            this->_solution.asignar_deposito_a_vendedor(d, v);

            // std::cout << std::endl;
            // std::cout << "prev cost: " << prev_cost << std::endl;
            // std::cout << "Cost: " << this->_solution.cost() << " -> " << best_cost << std::endl;
            this->_solution.recalc_cost();
            // std::cout << "Recalculated cost: " << this->_solution.cost() << std::endl;
        }
        else
        {
            break;
        }
        
    }
}

std::tuple<bool, double> Relocate::single_relocation(int v, int d){

    if (this->get_capacidad_deposito(d) > this->_instance.demanda(d, v))
    {

        double new_cost = this->_solution.cost() - this->_instance.cost(this->_solution.deposito_asignado_al_vendedor(v), v) + this->_instance.cost(d, v);

        if (new_cost < this->_solution.cost())
        {
            
            return std::make_tuple(true, new_cost);
        }
    }
    return std::make_tuple(false, 0);
}
