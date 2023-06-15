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

int GapInstance::c(int i) const {
    // Capacidad del depósito i
    return this->_c[i];
}

int GapInstance::d(int i, int j) const {
    // Demanda del vendedor j para el depósito i
    return this->_d[i][j];
}

int GapInstance::cost(int i, int j) const {
    // Costo de asignar el vendedor j al depósito i
    return this->_cost[i][j];
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

    input_file >> this->_n >> this->_m;

    std::cout << "n: " << this->_n << std::endl;
    std::cout << "m: " << this->_m << std::endl;

    this->_cost = std::vector<std::vector<int>>(this->_m, std::vector<int>(this->_n, 0));
    this->_d = std::vector<std::vector<int>>(this->_m, std::vector<int>(this->_n, 0));
    this->_c = std::vector<int>(this->_m, 0);

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
