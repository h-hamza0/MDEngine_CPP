#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <fstream>
#include <vector>
#include <Simulation.h>
using namespace std;

class Utility{
public:
    void check_and_update(Simulation &simulation);
    void com_removal(Simulation &Simulation);
    KineticResult find_ke_temp(Simulation &Simulation, int mass);
    void velocity_scaling(Simulation &Simulation);
};