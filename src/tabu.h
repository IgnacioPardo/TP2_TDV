#ifndef TABU_H
#define TABU_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Tabu : public Solver {
    public:
        using Solver::Solver;
        ~Tabu();
        virtual std::string get_name() const override;

        void solve();
};

#endif // Tabu_H
