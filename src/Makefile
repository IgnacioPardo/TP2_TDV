CC = g++
CFLAGS = -std=c++17
SRC = main.cpp gap_instance.cpp gap_solution.cpp solver.cpp greedy_mincost.cpp binpacking.cpp swap.cpp relocate.cpp random_destroyer.cpp greedy_randomized.cpp binpacking_randomized.cpp tabu.cpp rvnd.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = gap_simulator

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
