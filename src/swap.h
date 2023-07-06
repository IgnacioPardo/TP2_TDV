#ifndef SWAP_H
#define SWAP_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Swap : public AlteringSolver {
    public:
        using AlteringSolver::AlteringSolver;
        ~Swap();

        void solve(GapSolution solution);

        void local_search();
        void perform_swap();

        std::tuple<bool, double> single_swap(int v1, int v2, int d1, int d2);
        std::tuple<bool, double> swap_over_unasigned(int va, int vu, int da);

        void do_swap(int v1, int v2, int d1, int d2);

    private:
        int get_mejor_deposito(int j);
};

#endif // SWAP_H
