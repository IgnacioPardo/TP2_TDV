#ifndef GAP_SOLUTION_H
#define GAP_SOLUTION_H

#include<set>
#include<vector>
#include<string>
#include<fstream>
#include<iostream>

class GapSolution {
    public:
        GapSolution();
        GapSolution(int n, int m);
        ~GapSolution();

        // Getters

        // Cantidad de vendedores
        int n() const;

        // Cantidad de depósitos
        int m() const;

        // Depósito asignado al vendedor j
        int deposito_asignado_al_vendedor(int j) const;

        // Vendedores asignados al depósito i
        std::vector<int> vendedores_asignados_al_deposito(int i) const;

        // Costo total de la solución
        int cost() const;

        // Setters

        // Asignación del vendedor j al depósito i
        void asignar_deposito_a_vendedor(int i, int j);

        // Set cantidad de vendedores
        void set_n(int n);

        // Set cantidad de depósitos
        void set_m(int m);

        // Set costo total de la solución
        void set_cost(int cost);

        friend std::ostream& operator<<(std::ostream& os, const GapSolution& solution);

    private:
        // Cantidad de vendedores
        int _n;

        // Cantidad de depósitos
        int _m;

        // Costo total de la solución
        int _cost;

        // Deposito asignado al vendedor j
        std::vector<int> _deposito_asignado_a_vendedor;

        // Asignación de vendedores a depósitos
        std::vector<std::set<int>> _vendedores_asignados_a_deposito;
        
};

#endif // GAP_SOLUTION_H
