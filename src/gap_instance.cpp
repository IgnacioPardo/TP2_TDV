#include "gap_instance.h"

GapInstance::GapInstance() {
    this->_n = 0;
    this->_m = 0;
}

GapInstance::GapInstance(std::string filename) {
    _read(filename);
}

GapInstance::~GapInstance() {}

int GapInstance::n() const {
    // Cantidad de vendedores
    return this->_n;
}

int GapInstance::m() const {
    // Cantidad de depósitos
    return this->_m;
}

double GapInstance::capacidad(int i) const{
    // Capacidad del depósito i
    return this->_c[i];
}

double GapInstance::demanda(int i, int j) const{
    // Demanda del vendedor j para el depósito i
    return this->_d[i][j];
}

double GapInstance::cost(int i, int j) const{
    // Costo de asignar el vendedor j al depósito i
    if (i == -1){
        double max_cost = this->_cost[0][j];
        for (int k = 0; k < this->_m; k++){
            max_cost = std::max(max_cost, this->_cost[k][j]);
        }
        return max_cost * 3;
    }
    return this->_cost[i][j];
}

double GapInstance::penalizacion(int j) const{
    // Penalización del vendedor j
    double max_cost = this->_cost[0][j];
    for (int k = 1; k < this->_m; k++){
        max_cost = std::max(max_cost, this->_cost[k][j]);
    }
    return 3 * max_cost;
}

void GapInstance::_read(std::string filename){
    /*
    *    number of depositos (m), number of vendedores (n)
    !        for each deposito i (i=1,...,m) in turn:
    *            costo de asignar el vendedor j al deposito i (j=1,...,n)
    !        for each deposito i (i=1,...,m) in turn:
    *            demanda del vendedor j para el deposito i (j=1,...,n)
    *        capacidad del deposito i (i=1,...,m)
    */

    std::ifstream input_file(filename);

    input_file >> this->_m >> this->_n;

    this->_cost = std::vector<std::vector<double>>(this->_m, std::vector<double>(this->_n, 0));
    this->_d = std::vector<std::vector<double>>(this->_m, std::vector<double>(this->_n, 0));
    this->_c = std::vector<double>(this->_m, 0);

    // Matriz de _cost
    // _m x _n
    // Se lee uno por uno y se va llenando la matriz

    int cant = this->_n * this->_m;
    int i = 0;
    int j = 0;

    while (i < cant) {
        input_file >> this->_cost[j][i % this->_n];
        i++;
        if (i % this->_n == 0) {
            j++;
        }
    }    

    // Matriz _d

    i = 0;
    j = 0;

    while (i < cant) {
        input_file >> this->_d[j][i % this->_n];
        i++;
        if (i % this->_n == 0) {
            j++;
        }
    }

    // Vector _c

    for (int i = 0; i < this->_c.size(); i++) {
        input_file >> this->_c[i];
    }
    
}
