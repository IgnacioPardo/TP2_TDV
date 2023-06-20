#ifndef RELOCATE_H
#define RELOCATE_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Relocate : public Solver {
    public:
        using Solver::Solver;   
        ~Relocate();

        void solve();
        int perform_relocation(GapSolution &solution, int tries);
    
    private:
        int get_mejor_deposito(int j);
};

#endif // RELOCATE_H
