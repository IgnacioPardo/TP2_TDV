#ifndef SWAP_H
#define SWAP_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Swap : public Solver {
    public:
        using Solver::Solver;   
        ~Swap();

        void solve();
        int perform_swap(GapSolution &solution, int tries);
    
    private:
        int get_mejor_deposito(int j);
};

#endif // SWAP_H