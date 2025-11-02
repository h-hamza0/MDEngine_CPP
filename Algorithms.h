#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include "Simulation.h"
#pragma once
using namespace std;

class Algorithms{
public:
    void velocity_verlet(Simulation &simulation);
    void velocity_scaling(Simulation &simulation, float t_bulk);
private:
};