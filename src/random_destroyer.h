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
    
        void set_is_unassigner(bool is_unassigner);
    
    private:
        void perform_destruction();
        bool _is_unassigner = false;
};

#endif // RANDOMDESTROY_H
