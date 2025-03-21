# Centralized First Mile Logistics Optimization 🚚📦

## Overview
This repository presents our solution for the TP2 project “Centralized First Mile Logistics” developed for the *Digital Technology V: Algorithm Design* course. In this project, we model and solve the problem of assigning vendors to warehouses (depots) in a centralized manner, aiming to minimize the total travel distance while respecting the capacity constraints of each depot. Our work includes the design, implementation, and experimental evaluation of various heuristics, local search operators, and metaheuristics for the Generalized Assignment Problem (GAP).

## Team Members
- Tadeo Yapoudjian
- Luca Mazzarello
- Ignacio Pardo

## Problem Description
The challenge arises from the inefficiencies in the current logistics model where vendors choose their own depot for package delivery. This often results in uneven utilization of depot capacity, leading to additional costs and poor service quality. Our objective is to design a centralized assignment model where each vendor is regularly assigned to a specific depot so that:
- The total distance traveled by vendors is minimized.
- Each depot’s maximum capacity is not exceeded.
- In cases where a feasible assignment is not possible, a penalty is applied.

The problem is modeled as a Generalized Assignment Problem (GAP) where:
- **N**: Set of vendors
- **M**: Set of depots (each with a capacity limit)
- **dij**: Demand (or travel distance) if vendor *j* is assigned to depot *i*
- **ci**: Maximum capacity of depot *i*
- **Objective**: Minimize the total assignment cost (i.e., total travel distance), considering penalties for unassigned vendors.

## Our Approach
We implemented and compared several methods to solve the GAP, including:

### Heuristic Methods
- **Greedy MinCost Heuristic**: Assigns each vendor to the nearest depot with available capacity. If no depot can accommodate the vendor, a penalty is applied.
- **Bin Packing Heuristic**: Optimizes the use of depot capacity by assigning vendors to minimize the remaining free space in each depot.

### Local Search Operators
- **Swap Operator**: Exchanges the depot assignments of two vendors if the swap reduces the total travel distance while maintaining capacity constraints.
- **Relocate Operator**: Reassigns a vendor to another depot with lower associated travel cost, provided the depot can handle the additional demand.

### Metaheuristic Approaches
- **Tabu Search**: Applies a swap-based neighborhood search with memory structures to avoid cycling and escape local minima.
- **Randomized Variable Neighbourhood Descent (RVND)**: Combines multiple local search operators in a randomized manner to explore diverse neighborhoods and improve the solution quality.

## Experimental Evaluation
We conducted extensive experiments using a set of benchmark instances (labeled “A”, “B”, and “E”) as well as a real-world instance (with 1100 vendors and 310 depots). For each instance, we evaluated:
- **Solution Cost**: Total travel distance (with penalties if necessary).
- **Execution Time**: Time required for each algorithm.
- **Assignment Feasibility**: Number of vendors successfully assigned to depots.

Key findings include:
- Constructive heuristics (Greedy and Bin Packing) quickly generate initial solutions, though their quality varies.
- Local search operators (Swap and Relocate) improve the initial solutions, with the Bin Packing based approach generally outperforming the Greedy approach.
- Metaheuristics (Tabu Search and RVND) further enhance solution quality, with RVND often achieving the best results, especially on the real-world instance.
- Our experiments also analyzed the trade-offs between execution time and solution quality.

## How to Run the Project
1. **Compilation & Execution:**
   - The implementation is in C++.
   - Follow the provided instructions in the repository (and in the accompanying report) to compile and run the code.
2. **Input/Output:**
   - The program requires at least the following parameters:
     - Input file name containing the instance.
     - Output file name to write the solution.
     - Additional parameters as needed (please refer to the report for details).
3. **Output Format:**
   - Each line of the output file corresponds to a depot.
   - Each line contains a space-separated list of vendor indices assigned to that depot.

## Conclusion
Our study demonstrates the effectiveness of combining constructive heuristics with local search and metaheuristic techniques to solve the centralized first mile logistics problem. The proposed methods not only reduce the total travel distance but also ensure efficient use of depot capacity. Our experimental results provide valuable insights into the trade-offs between solution quality and computational time, supporting practical decision-making for logistics operations.

## Acknowledgements
We extend our sincere thanks to our instructors, mentors, and peers for their guidance and support throughout this project. Special thanks to ThunderPack for providing the real-world context that inspired this work.

Happy optimizing! 🚀📊
