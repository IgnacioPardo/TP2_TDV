#ifndef GREEDY_H
#define GREEDY_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Greedy : public Solver {
    public:
        using Solver::Solver;
        ~Greedy();

        void solve();

};

#endif // GREEDY_H
