#ifndef RELOCATE_H
#define RELOCATE_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Relocate : public AlteringSolver {
    public:
        using AlteringSolver::AlteringSolver;   
        ~Relocate();
        
        void set_solution(GapSolution solution);

        void solve(GapSolution solution);
        std::vector<std::tuple<int, int, double>> neighbourhood();
        std::tuple<bool, double> single_relocation(int v, int d);
        void do_relocation(int v, int d);
        void local_search();

        virtual std::string get_name() const override;

    private:
        int get_mejor_deposito(int j);
};

#endif // REALOCATE_H
