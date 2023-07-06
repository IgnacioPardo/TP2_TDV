#include "solver.h"

Solver::Solver(){
    this->_instance_loaded = false;
    
    this->_solution_time = 0;

    this->_status = 0;
    this->_solution = GapSolution();
}

Solver::Solver(GapInstance instance) {
    this->_instance = instance;
    this->_instance_loaded = true;
    
    this->_solution_time = 0;
    
    this->_status = 0;
    this->_solution = GapSolution(instance);
}

Solver::~Solver() {}

void Solver::set_instance(GapInstance instance) {
    this->_instance = instance;
    this->_instance_loaded = true;

    this->_solution.set_n(instance.n());
    this->_solution.set_m(instance.m());
}

double Solver::get_cost() const
{
    return this->_solution.cost();
}

GapSolution Solver::get_solution() const {
    return this->_solution;
}

double Solver::get_capacidad_deposito(int i) {
    double capacidad = this->_instance.capacidad(i);
    // O(n)
    for (int j : this->_solution.vendedores_asignados_al_deposito(i)) {
        capacidad -= this->_instance.demanda(i, j);
    }
    return capacidad;
}
