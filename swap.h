#ifndef TWOOPT_H
#define TWOOPT_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class Two_opt : public Solver {
    public:
        using Solver::Solver;   
        ~Two_opt();

        void solve();
    
    private:
        int get_mejor_deposito(int j);
};

#endif // TWOOPT_H
