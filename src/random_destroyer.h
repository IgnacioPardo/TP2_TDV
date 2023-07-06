#ifndef RANDOMDESTROYER_H
#define RANDOMDESTROYER_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"

class RandomDestroyer : public AlteringSolver {
    public:
        using AlteringSolver::AlteringSolver;
        ~RandomDestroyer();

        void solve(GapSolution solution);
        virtual std::string get_name() const override;
    
    private:
        void perform_destruction();
};

#endif // RANDOMDESTROY_H
