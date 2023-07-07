#ifndef RVND_H
#define RVND_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class RVND : public Solver {
    public:
        using Solver::Solver;
        ~RVND();
        virtual std::string get_name() const override;

        void solve();
};

#endif // RVND_H
