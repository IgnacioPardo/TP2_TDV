#ifndef GREEDY_H
#define GREEDY_H

#include "gap_solution.h"
#include "gap_instance.h"

class Greedy {
    public:
        Greedy();
        Greedy(GapInstance instance);
        ~Greedy();

        void solve();

        // Getters

        // Get Solution
        GapSolution get_solution() const;

        // Get Cost
        int get_cost() const;

        // Set Instance
        void set_instance(GapInstance instance);

    private:
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

        // Greedy
        void greedy();
};

#endif // GREEDY_H
