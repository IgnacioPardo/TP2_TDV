#ifndef RELOCATE_H
#define RELOCATE_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Relocate : public Solver {
    public:
        using Solver::Solver;   
        ~Relocate();

        void solve(GapSolution solution);
        void perform_relocation(int tries);
        GapSolution single_relocation(int v, int d, GapSolution sol);
        void local_search();

    private:
        int get_mejor_deposito(int j);
};

#endif // REALOCATE_H
