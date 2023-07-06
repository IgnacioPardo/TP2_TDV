#ifndef GREEDY_MINCOST_H
#define GREEDY_MINCOST_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class GreedyMinCost : public Solver {
    public:
        using Solver::Solver;   
        ~GreedyMinCost();

        void solve();

        virtual std::string get_name() const override;
    
    private:
        int get_mejor_deposito(int j);
};

#endif // GREEDY_MINCOST_H
