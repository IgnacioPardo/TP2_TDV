#ifndef SOLVER_H
#define SOLVER_H

#include "gap_solution.h"
#include "gap_instance.h"

class Solver {
    public:
        explicit Solver();
        explicit Solver(GapInstance instance);
        ~Solver();

        void solve();

        // Getters

        // Get Solution
        GapSolution get_solution() const;

        // Get Cost
        int get_cost() const;

        // Set Instance
        void set_instance(GapInstance instance);

    protected:
        // Instance
        GapInstance _instance;
        bool _instance_loaded;

        // Solution
        GapSolution _solution;
        int _status;

        // Cost
        int _cost;

        // Solution Time
        double _solution_time;
};

#endif // GREEDY_H
