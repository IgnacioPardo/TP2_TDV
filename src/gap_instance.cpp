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
        double max_cost = 0;
        for (int k = 0; k < this->_m; k++){
            max_cost = std::max(max_cost, this->_cost[k][j]);
        }
        return max_cost * 3;
    }
    return this->_cost[i][j];
}

double GapInstance::penalizacion(int j) const{
    double demanda_maxima_vj = 0;
    for (int i = 0; i < this->_m; i++){
        double c_dem = demanda(i, j);
        if (c_dem > demanda_maxima_vj){
            demanda_maxima_vj = c_dem;
        }
    }
    
    return 3 * demanda_maxima_vj;
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

    // Fill _cost matrix
    // The matrix is _m x _n
    // The data is not in the same order as the matrix
    // While reading the file, we will fill the matrix item by item

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

    // Fill _d matrix

    i = 0;
    j = 0;

    while (i < cant) {
        input_file >> this->_d[j][i % this->_n];
        i++;
        if (i % this->_n == 0) {
            j++;
        }
    }

    // Fill _c vector

    for (int i = 0; i < this->_c.size(); i++) {
        input_file >> this->_c[i];
    }
    
}
