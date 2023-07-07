#ifndef BINPACKING_H
#define BINPACKING_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class BinPacking : public Solver {
    public:
        using Solver::Solver;   
        ~BinPacking();

        virtual void solve();
        virtual std::string get_name() const override;

    protected:
        int get_mejor_deposito(int j);
    
};

#endif // BINPACKING_H
