CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall

SRC = main.cpp Simulation.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = simulation

all: $(EXEC)

$(EXEC) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)