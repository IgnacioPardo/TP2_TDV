#include "gap_solution.h"

GapSolution::GapSolution() {
    this->_n = 0;
    this->_m = 0;
    this->_cost = 0;
    this->_time = 0;
}

GapSolution::GapSolution(int n, int m) {
    this->_n = n;
    this->_m = m;
    this->_cost = 0;
    this->_time = 0;

    this->_deposito_asignado_a_vendedor = std::vector<int>(n, -1);
    this->_vendedores_asignados_a_deposito = std::vector<std::set<int>>(m);
}

GapSolution::~GapSolution() {}

int GapSolution::n() const {
    // Cantidad de vendedores
    return this->_n;
}

int GapSolution::m() const {
    // Cantidad de depósitos
    return this->_m;
}

int GapSolution::deposito_asignado_al_vendedor(int j) const {
    // Depósito asignado al vendedor j
    return this->_deposito_asignado_a_vendedor[j];
}

std::vector<int> GapSolution::vendedores_asignados_al_deposito(int i) const {
    // Vendedores asignados al depósito i
    std::vector<int> vendedores_asignados;
    for (int j : this->_vendedores_asignados_a_deposito[i]) {
        vendedores_asignados.push_back(j);
    }
    return vendedores_asignados;
}

double GapSolution::cost() const {
    // Costo total de la solución
    return this->_cost;
}

void GapSolution::asignar_deposito_a_vendedor(int i, int j) {
    // Asignación del vendedor j al depósito i
    
    this->_deposito_asignado_a_vendedor[j] = i;
    this->_vendedores_asignados_a_deposito[i].insert(j);

}

void GapSolution::desasignar_deposito_de_vendedor(int i, int j) {
    // Desasignación del vendedor j del depósito i
    
    this->_deposito_asignado_a_vendedor[j] = -1;
    this->_vendedores_asignados_a_deposito[i].erase(j);
    
}


void GapSolution::set_n(int n) {
    // Set cantidad de vendedores
    this->_n = n;
    this->_deposito_asignado_a_vendedor = std::vector<int>(n, -1);
}

void GapSolution::set_m(int m) {
    // Set cantidad de depósitos
    this->_m = m;
    this->_vendedores_asignados_a_deposito = std::vector<std::set<int>>(m);
}

void GapSolution::set_cost(int cost) {
    // Set costo total de la solución
    this->_cost = cost;
}

void GapSolution::set_time(double time) {
    // Set tiempo en resolver el problema
    this->_time = time;
}

double GapSolution::time() const {
    // Tiempo en resolver el problema
    return this->_time;
}

std::ostream& operator<<(std::ostream& os, const GapSolution& solution){

    os << "n: " << solution.n() << std::endl;
    os << "m: " << solution.m() << std::endl;
    os << "cost: " << solution.cost() << std::endl;
    os << "time: " << solution.time() << std::endl;

    /* for (int i = 0; i < solution.n(); i++) {
        os << "vendedor " << i << " -> deposito " << solution.deposito_asignado_al_vendedor(i) << std::endl;
    } */

    for (int i = 0; i < solution.m(); i++) {
        os << "deposito " << i << " -> ";
        for (int j : solution.vendedores_asignados_al_deposito(i)) {
            os << j << " ";
        }
        os << std::endl;
    }

    return os;
}
