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

        // Get Solver Name
        virtual std::string get_name() const; 

        // Get Solution
        GapSolution get_solution() const;

        // Get Cost
        double get_cost() const;

        // Set Instance
        void set_instance(GapInstance instance);

        double get_capacidad_deposito(int i);

    protected:
        // Instance
        GapInstance _instance;
        bool _instance_loaded;

        // Solution
        GapSolution _solution;
        int _status;

        // Solution Time
        double _solution_time;
};

class AlteringSolver : public Solver {
    public:
        using Solver::Solver;
        void solve(GapSolution solution);

        virtual std::string get_name() const override;
};

#endif // GREEDY_H
