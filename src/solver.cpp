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

void Solver::solve() {
    this->_solution_time = 0;
    this->_status = 0;
}

std::string Solver::get_name() const {
    return "Solver";
}

void Solver::set_instance(GapInstance instance) {
    /*
    *   Setea la instancia a resolver.
    */
    this->_instance = instance;
    this->_instance_loaded = true;

    this->_solution = GapSolution(instance);
}

double Solver::get_cost() const
{
    return this->_solution.cost();
}

GapSolution Solver::get_solution() const {
    return this->_solution;
}

double Solver::get_capacidad_deposito(int i) {
    return this->_solution.capacidad_actual_deposito(i);
}

void AlteringSolver::solve(GapSolution solution) {
    this->_solution = solution;
    this->_solution_time = 0;
    this->_status = 0;
}

std::string AlteringSolver::get_name() const {
    return "AlteringSolver";
}
