#ifndef BINPACKING_H
#define BINPACKING_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class BinPacking : public Solver {
    public:
        using Solver::Solver;   
        ~BinPacking();

        void solve();

    private:
        int get_mejor_deposito(int j);
    
};

#endif // BINPACKING_H
