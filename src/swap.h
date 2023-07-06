#ifndef SWAP_H
#define SWAP_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Swap : public AlteringSolver {
    public:
        using AlteringSolver::AlteringSolver;
        ~Swap();


        void set_solution(GapSolution solution);
        void solve(GapSolution solution);
        virtual std::string get_name() const override;

        void local_search();
        std::vector<std::tuple<int, int, int, int, double>> neighbourhood();

        std::tuple<bool, double> single_swap(int v1, int v2, int d1, int d2);
        std::tuple<bool, double> swap_over_unasigned(int va, int vu, int da);

        void do_swap(int v1, int v2, int d1, int d2);
};

#endif // SWAP_H
