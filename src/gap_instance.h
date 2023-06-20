#ifndef GAP_INSTANCE_H
#define GAP_INSTANCE_H

#include<vector>
#include<string>
#include<fstream>
#include<iostream>

/*

El problema descrito anteriormente puede ser modelado mediante el Problema de Asignación Generalizada (GAP, Generalized Assignment Problem), que en su versión más general puede ser formulado de la siguiente forma. Sea N el conjunto de vendedores, donde N = {1,...,n}, y M el conjunto de depósitos, donde M = {1,...,m}. Cada depósito i ∈ M tiene una capacidad máxima de recepción ci, medida en cantidad de unidades. Dado un vendedor j ∈ N y un depósito i ∈ M, dij denota la demanda, también medida en cantidad de unidades, a utilizar, y cij representa el costo incurrido si j es asignado a i. Con estas definiciones, podemos representar una solución como una colección de conjuntos Γ1,...,Γm, donde cada Γi es un subconjunto de vendedores asignados al depósito i ∈ M. Luego, el GAP consiste en:

Asignar cada vendedor j ∈ N a exactamente un depósito i ∈ M, es decir, asegurar que la intersección entre Γi y Γk sea vacía si i ≠ k, donde i, k ∈ M.
La capacidad de cada depósito no debe ser excedida, lo cual se expresa como la restricción siguiente para cada i ∈ M:
Σdij ≤ ci para todo j ∈ Γi.
Minimizar el costo total de la asignación, dado por la siguiente expresión:
ΣΣcij para i = 1 hasta n y j ∈ Γi.

*/

class GapInstance {

    public:
        GapInstance();
        GapInstance(std::string filename);
        ~GapInstance();

        // Getters

        // Cantidades de vendedores
        int n() const;

        // Cantidades de depósitos
        int m() const;

        // Capacidad del depósito i
        double capacidad(int i) const;

        // Demanda del vendedor j para el depósito i
        double demanda(int i, int j) const;

        // Costo de asignar el vendedor j al depósito i
        double cost(int i, int j) const;

    private:
        // Cantidad de vendedores
        int _n;

        // Cantidad de depósitos
        int _m;

        // Capacidad del depósito i
        std::vector<double> _c;

        // Demanda del vendedor j para el depósito i
        std::vector<std::vector<double>> _d;

        // Costo de asignar el vendedor j al depósito i
        std::vector<std::vector<double>> _cost;

        void _read(std::string filename);
};

#endif // GAP_INSTANCE_H
