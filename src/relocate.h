#ifndef RELOCATE_H
#define RELOCATE_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Relocate : public AlteringSolver {
    public:
        using AlteringSolver::AlteringSolver;   
        ~Relocate();

        void solve(GapSolution solution);
        void perform_relocation(int tries);
        std::tuple<bool, double> single_relocation(int v, int d);
        void local_search();

    private:
        int get_mejor_deposito(int j);
};

#endif // REALOCATE_H
