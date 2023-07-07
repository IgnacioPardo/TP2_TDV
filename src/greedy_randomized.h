#ifndef GREEDY_RANDOMIZED_H
#define GREEDY_RANDOMIZED_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"
#include "greedy_mincost.h"

class GreedyRandomized : public GreedyMinCost {
    public:
        using GreedyMinCost::GreedyMinCost;
        ~GreedyRandomized();

        void solve() override;

        virtual std::string get_name() const override;
};

#endif // GREEDY_RANDOMIZED_H
