#ifndef META_H
#define META_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Meta : public Solver {
    public:
        using Solver::Solver;
        ~Meta();
        virtual std::string get_name() const override;

        void solve();
};

#endif // META_H
