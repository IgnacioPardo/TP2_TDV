#ifndef BINPACKING_RANDOMIZED_H
#define BINPACKING_RANDOMIZED_H

#include "solver.h"

#include "gap_solution.h"
#include "gap_instance.h"
#include "binpacking.h"

class BinPackingRandomized : public BinPacking {
    public:
        using BinPacking::BinPacking;
        ~BinPackingRandomized();

        void solve() override;

        virtual std::string get_name() const override;
    
};

#endif // BINPACKING_RANDOMIZED_H
