#include "binpacking.h"

#include <tuple>

BinPacking::~BinPacking() {}

std::string BinPacking::get_name() const {
    return "BinPacking";
}

void BinPacking::solve(){
    /*
    *   A cada vendedor j le asigno el deposito i cuya capacidad al restarle la demanda del vendedor j sea la menor
    */

    auto start = std::chrono::steady_clock::now();
    
    // Por cada vendedor j
    // Busco el deposito i cuya capacidad al restarle la demanda del vendedor j sea la menor

    for (int j = 0; j < this->_instance.n(); j++) {
        int deposito = this->get_mejor_deposito(j);
        
        if (deposito != -1)   
            this->_solution.asignar_deposito_a_vendedor(deposito, j);
        
    }

    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();

    this->_solution.set_time(this->_solution_time);
}

int BinPacking::get_mejor_deposito(int j) {
    /*
    *   Devuelve el deposito que tenga menor espacio restante luego de mi demanda y que cumpla con la demanda del vendedor j.
    *   Si no hay deposito que cumpla con la demanda del vendedor j, devuelve -1.
    */

    int mejor_deposito = -1;
    int mejor_costo = -1;

    // Busco el deposito i cuya capacidad al restarle la demanda del vendedor j sea la menor

    std::vector<std::tuple<int, double>> capacidades_con_j(this->_instance.m());

    for (int deposito = 0; deposito < this->_instance.m(); deposito++) {
        double cap = this->get_capacidad_deposito(deposito) - this->_instance.demanda(deposito, j);

        capacidades_con_j[deposito] = std::make_tuple(cap, deposito);
    }

    // Ordenar capacidades de mayor a menor por el primer elemento de la tupla
    std::sort(capacidades_con_j.begin(), capacidades_con_j.end(), [](const std::tuple<int, double> &a, const std::tuple<int, double> &b) -> bool
              { return std::get<0>(a) > std::get<0>(b); });

    // Si la capacidad del primer elemento de la lista ordenada es menor a 0, no hay deposito que cumpla con la demanda del vendedor j
    if (std::get<0>(capacidades_con_j[0]) < 0) {
        return -1;
    }

    // Si la capacidad del primer elemento de la lista ordenada es mayor a 0, devuelvo el deposito del primer elemento de la lista ordenada
    return std::get<1>(capacidades_con_j[0]);
}

    
    
