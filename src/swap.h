#ifndef SWAP_H
#define SWAP_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Swap : public Solver {
    public:
        using Solver::Solver;   
        ~Swap();

        void solve(GapSolution solution);
        void local_search();
        void perform_swap();
        GapSolution single_swap(int v1, int v2, GapSolution sol);
        GapSolution swap_over_unasigned(int va, int vu, GapSolution sol);

    private: 
        int get_mejor_deposito(int j);
};

#endif // SWAP_H
