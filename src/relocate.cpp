#include "relocate.h"
#include "greedy_mincost.h"
#include "binpacking.h"
#include "swap.h"

#include <random>
#include <numeric>

Relocate::~Relocate() {}

std::string Relocate::get_name() const {
    return "Relocate";
}

void Relocate::solve(GapSolution solution){
    /*
    *  Aplica la heuristica de bisqueda local Relocate a la solución pasada por parámetro.
    *  La solución pasada por parámetro es la solución inicial debe ser una solución factible.s
    */

    auto start = std::chrono::steady_clock::now();

    this->_solution = solution;
    this->_solution_time = solution.time();

    this->local_search();

    auto end = std::chrono::steady_clock::now();
    this->_solution_time += std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

std::vector<std::tuple<int, int, double>> Relocate::neighbourhood(){
    /*
    *   Genera los vecinos de la solución actual
    *   Los vecinos son todas aquellas soluciones que puedo obtener de reubicar un vendedor en otro depósito.
    *   Reservo el primer lugar del vector para guardar la mejor solución encontrada.
    *   Devuelve un vector de tuplas de la forma (v, d, costo)
    *   Si no hay vecinos factibles, devuelve un vector de tuplas vacío.
    */

    double min_cost = std::numeric_limits<double>::max();
    std::vector<std::tuple<int, int, double>> relocation_w_cost = std::vector<std::tuple<int, int, double>>();

    // Agrego un falso vecino inicial como espacio para guardar la mejor solución
    // Me evita tener que ordenar el vecindario
    relocation_w_cost.push_back(std::make_tuple(-1, -1, min_cost));
    // Si no se generan vecinos factibles, devuelvo un vector vacío

    // Genero todos los vecinos
    // O(n*m)
    for (int v = 0; v < this->_solution.n(); v++)
    {
        for (int d = 0; d < this->_solution.m(); d++)
        {

            // Solución actual
            if (this->_solution.deposito_asignado_al_vendedor(v) == d)
                continue;

            // Evaluar la factibilidad de la solución y su costo
            std::tuple<bool, double> relocation = this->single_relocation(v, d);

            // Si la solución es factible
            if (std::get<0>(relocation))
            {
                // Si la solución es mejor que la actual
                if (std::get<1>(relocation) < this->_solution.cost())
                {
                    // Agrego la solución a la lista de vecinos
                    relocation_w_cost.push_back(std::make_tuple(v, d, std::get<1>(relocation)));

                    if (std::get<1>(relocation) < min_cost)
                    {
                        min_cost = std::get<1>(relocation);
                        relocation_w_cost[0] = std::make_tuple(v, d, min_cost);
                    }
                }
            }
        }
    }

    if (relocation_w_cost.size() == 1)
        return std::vector<std::tuple<int, int, double>>();

    return relocation_w_cost;
}

void Relocate::local_search(){
    /*
    *  Genera los vecinos de la solución actual y se queda con el mejor.
    *  Repite hasta que no haya mejora.
    *  Los vecinos son todas aquellas soluciones que puedo obtener de realizar una reubicación de un vendedor a un depósito.
    */

    double prev_cost = 0;

    while (this->_solution.cost() < prev_cost || prev_cost == 0)
    {

        std::vector<std::tuple<int, int, double>> neighbours = this->neighbourhood();

        // Si hay vecinos
        if (neighbours.size() > 0)
        {
            // Ordeno los vecinos por costo
            // std::sort(neighbours.begin(), neighbours.end(), [](std::tuple<int, int, int> a, std::tuple<int, int, int> b) {
            //     return std::get<2>(a) < std::get<2>(b);
            // });

            // Registro el costo de la solución actual
            prev_cost = this->_solution.cost();
            
            // Elijo el mejor vecino
            std::tuple<int, int, double> best_relocation = neighbours[0];

            int v = std::get<0>(best_relocation);
            int d = std::get<1>(best_relocation);

            double best_cost = std::get<2>(best_relocation);

            // Realizo la reubicación
            this->do_relocation(v, d);

            this->_solution.set_cost(best_cost);
        }
        else
        {
            // No hay vecinos que mejoren la solución actual
            // Termina la búsqueda local
            break;
        }
        
    }
}

void Relocate::do_relocation(int v, int d){
    /*
    *  Realiza la reubicación de un vendedor a un depósito.
    *  La solución pasada por parámetro es la solución inicial debe ser una solución factible.
    */

    int prev_d = this->_solution.deposito_asignado_al_vendedor(v);

    if (prev_d != -1)
        this->_solution.desasignar_deposito_de_vendedor(prev_d, v);

    this->_solution.asignar_deposito_a_vendedor(d, v);
}

std::tuple<bool, double> Relocate::single_relocation(int v, int d){
    /*
    *  Devuelve true si la reubicación es factible, false en caso contrario.
    *  Devuelve el costo de la reubicación.
    */

    if (this->get_capacidad_deposito(d) > this->_instance.demanda(d, v))
    {

        double new_cost = this->_solution.cost() - this->_instance.cost(this->_solution.deposito_asignado_al_vendedor(v), v) + this->_instance.cost(d, v);
        return std::make_tuple(true, new_cost);
    }
    return std::make_tuple(false, 0);
}

void Relocate::set_solution(GapSolution solution)
{
    // Actualiza la solución actual.
    this->_solution = solution;
}
