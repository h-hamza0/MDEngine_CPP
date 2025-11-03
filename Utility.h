#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <fstream>
#include <vector>
#include "Simulation.h"
#pragma once
using namespace std;

class Simulation;

class Utility{
public:
    void check_and_update(Simulation &simulation);
    void com_removal(Simulation &Simulation);
    KineticResult find_ke_temp(Simulation &Simulation, int mass);
    void compute_velocity(Simulation &Simulation);
    void clear_force(Simulation &simulation);
    void find_acceleration(Simulation &simulation, float mass);
    ForceResult compute_force(Simulation &simulation);
};