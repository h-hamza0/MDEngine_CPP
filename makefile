CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall

SRC = main.cpp Simulation.cpp Algorithms.cpp Output.cpp Utility.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = simulation

TEST_SRC = tests/test_engine.cpp Simulation.cpp Utility.cpp Algorithms.cpp Output.cpp
TEST_EXEC = tests/test_engine

all: $(EXEC)

$(EXEC) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)

test: $(TEST_EXEC)
	@echo "Running tests..."
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -Itest $(TEST_SRC) -o $(TEST_EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)