#ifndef GAP_SOLUTION_H
#define GAP_SOLUTION_H

#include<set>
#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include "gap_instance.h"

class GapSolution {
    public:
        GapSolution();
        GapSolution(GapInstance instance);
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
        double cost() const;

        // Tiempo en resolver el problema
        double time() const;

        // Setters

        // Asignación del vendedor j al depósito i
        void asignar_deposito_a_vendedor(int i, int j);

        // Desasignación del vendedor j del depósito i
        void desasignar_deposito_de_vendedor(int i, int j);

        // Capacidad actual del depósito i
        int capacidad_actual_deposito(int i) const;

        // Set costo total de la solución
        void set_cost(double cost);

        // Seteo instancia
        void set_instance(GapInstance instance);

        // Set cantidad de vendedores
        void set_n(int n);

        // Set cantidad de depósitos
        void set_m(int m);

        // Set tiempo en resolver el problema
        void set_time(double time);

        // Copy
        GapSolution copy();

        std::vector<int> unassigned_vendors() const;

        friend std::ostream& operator<<(std::ostream& os, const GapSolution& solution);

        bool operator<(const GapSolution& other) const;
        bool operator>(const GapSolution& other) const;
        bool operator==(const GapSolution& other) const;
        bool operator!=(const GapSolution& other) const;
        bool operator<=(const GapSolution& other) const;
        bool operator>=(const GapSolution& other) const;

        void recalc_cost();

    private :
        // Instancia
        GapInstance _instance;

        // Cantidad de vendedores
        int _n;

        // Cantidad de depósitos
        int _m;

        // Costo total de la solución
        double _cost;

        // Deposito asignado al vendedor j
        std::vector<int> _deposito_asignado_a_vendedor;

        // Asignación de vendedores a depósitos
        std::vector<std::set<int>> _vendedores_asignados_a_deposito;
        
        // Tiempo en resolver el problema
        double _time;

        // Capacidad usada por cada depósito
        std::vector<int> _capacidad_usada_deposito;
};

#endif // GAP_SOLUTION_H
