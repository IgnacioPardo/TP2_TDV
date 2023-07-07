#include "gap_solution.h"

GapSolution::GapSolution() {
    this->_n = 0;
    this->_m = 0;
    this->_cost = 0;
    this->_time = 0;
}

GapSolution::GapSolution(GapInstance instance) {
    /*
    *   Inicializo la solución con todos los vendedores sin asignar
    *   y todos los depósitos sin usar
    *   Calculo el costo de la solución inicial
    *   Como todos los vendedores están sin asignar, el costo es la suma de las penalizaciones
    */
    

    this->_instance = instance;
    this->_n = instance.n();
    this->_m = instance.m();
    this->_time = 0;

    this->_deposito_asignado_a_vendedor = std::vector<int>(this->_n, -1);
    this->_vendedores_asignados_a_deposito = std::vector<std::set<int>>(this->_m);
    this->_capacidad_usada_deposito = std::vector<int>(this->_m, 0);
    
    this->recalc_cost();
}

GapSolution::~GapSolution() {}

void GapSolution::set_instance(GapInstance instance) {
    this->_instance = instance;

    this->_n = instance.n();
    this->_m = instance.m();

    this->_cost = 0;
    this->_time = 0;

    this->_deposito_asignado_a_vendedor = std::vector<int>(this->_n, -1);
    this->_vendedores_asignados_a_deposito = std::vector<std::set<int>>(this->_m);
    this->_capacidad_usada_deposito = std::vector<int>(this->_m, 0);

    this->recalc_cost();
}

int GapSolution::n() const {
    // Cantidad de vendedores
    return this->_n;
}

int GapSolution::m() const {
    // Cantidad de depósitos
    return this->_m;
}

int GapSolution::capacidad_actual_deposito(int i) const {
    // Capacidad actual del depósito i, es decir, la capacidad total menos la capacidad usada
    return this->_instance.capacidad(i) - this->_capacidad_usada_deposito[i];
}

int GapSolution::deposito_asignado_al_vendedor(int j) const {
    // Depósito asignado al vendedor j
    // Complejidad: O(1)
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

void GapSolution::set_cost(double cost) {
    // Setea el costo de la solución
    this->_cost = cost;
}

void GapSolution::recalc_cost() {
    // Costo total de la solución
    // Complejidad: O(n * m)

    double calculated_cost = 0;

    for (int v = 0; v < this->_n; v++) { // O(n)
        int d = this->_deposito_asignado_a_vendedor[v];
        if (d != -1) {
            calculated_cost += this->_instance.cost(d, v);
        }
        else {
            // O(m)
            calculated_cost += this->_instance.penalizacion(v);
        }
    }
    
    this->_cost = calculated_cost;
}

double GapSolution::cost() const {
    // Costo total de la solución
    return this->_cost;
}

void GapSolution::asignar_deposito_a_vendedor(int i, int j) {
    // Asignación del vendedor j al depósito i

    if (this->_deposito_asignado_a_vendedor[j] == -1) {
        this->_cost -= this->_instance.penalizacion(j);
    }
    
    this->_deposito_asignado_a_vendedor[j] = i;
    this->_vendedores_asignados_a_deposito[i].insert(j);

    this->_capacidad_usada_deposito[i] += this->_instance.demanda(i, j);

    this->_cost += this->_instance.cost(i, j);
}

void GapSolution::desasignar_deposito_de_vendedor(int i, int j) {
    // Desasignación del vendedor j del depósito i
    
    this->_deposito_asignado_a_vendedor[j] = -1;

    // Del set de vendedores asignados al depósito i, borramos al vendedor j como item
    auto it = this->_vendedores_asignados_a_deposito[i].find(j);
    if (it != this->_vendedores_asignados_a_deposito[i].end()){
        this->_vendedores_asignados_a_deposito[i].erase(it);
    }

    this->_capacidad_usada_deposito[i] -= this->_instance.demanda(i, j);

    this->_cost -= this->_instance.cost(i, j);
    this->_cost += this->_instance.penalizacion(j);
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

void GapSolution::set_time(double time) {
    // Set tiempo en resolver el problema
    this->_time = time;
}

double GapSolution::time() const {
    // Tiempo en resolver el problema
    return this->_time;
}

std::vector<int> GapSolution::unassigned_vendors() const {
    // Vendedores sin asignar
    std::vector<int> unassigned_vendors_vec;
    for (int i = 0; i < this->_n; i++)
    {
        if (this->_deposito_asignado_a_vendedor[i] == -1)
        {
            unassigned_vendors_vec.push_back(i);
        }
    }
    return unassigned_vendors_vec;
}

std::ostream& operator<<(std::ostream& os, const GapSolution& solution){

    os << "Cantidad de vendedores (n): " << solution.n() << std::endl;
    os << "Cantidad de depositos (m): " << solution.m() << std::endl;
    os << "Cost: " << solution.cost() << std::endl;
    os << "Time: " << solution.time() << std::endl;

    for (int i = 0; i < solution.m(); i++) {
        os << "deposito " << i << " -> ";
        for (int j : solution.vendedores_asignados_al_deposito(i)) {
            os << j << " ";
        }
        os << std::endl;
    }

    std::cout << "Sin asignar: ";
    for (int i : solution.unassigned_vendors()) {
        std::cout << i << " ";
    }

    return os;
}

// Comparación de soluciones

bool GapSolution::operator<(const GapSolution& other) const {
    return this->cost() < other.cost();
}

bool GapSolution::operator>(const GapSolution& other) const {
    return this->cost() > other.cost();
}

bool GapSolution::operator==(const GapSolution& other) const {
    return this->cost() == other.cost();
}

bool GapSolution::operator!=(const GapSolution& other) const {
    return this->cost() != other.cost();
}

bool GapSolution::operator<=(const GapSolution& other) const {
    return this->cost() <= other.cost();
}

bool GapSolution::operator>=(const GapSolution& other) const {
    return this->cost() >= other.cost();
}

// Copia de solucion

GapSolution GapSolution::copy(){
    // Devuelve una copia de la solución
    GapSolution solution(this->_instance);
    solution.set_time(this->_time);
    
    // O(n*m)
    for (int i = 0; i < this->_n; i++) { // O(n)
        if (this->_deposito_asignado_a_vendedor[i] != -1)
            solution.asignar_deposito_a_vendedor(this->_deposito_asignado_a_vendedor[i], i); // O(m)
    }

    return solution;
}
