#include <iostream>
#include <vector>
#include <variant>
#include "Simulation.h"
#include "Algorithms.h" 
#include "Utility.h"
#include "Output.h"
// include other files below here
using namespace std;

using VarType = std::variant<float, vector<float>>;

int main() {
    Simulation init;
    Algorithms* algos;
    Utility util;
    ForceResult fc;
    KineticResult ke;
    Output* out;
    init.read_input("RUN_PARAMETERS.cfg");
    cout << "Read input" << endl;
    init.read_system("argon.csv");
    cout << "Read system" << endl;
    float mass = 39.9;
    int PRESS_COUNT = 0;
    float box_VOL = init.box_L * init.box_B * init.box_H;
    for (int i = 1; i < init.TOT_STEPS; i++){
        init.integrate(algos, util, PRESS_COUNT, i, fc, mass, ke, out, box_VOL);
        cout << "Exec = " << (i * 100) / init.TOT_STEPS << " %, PE = " << init.EWW << " T = " << init.T_BULK << endl;
    }
    cout << "End program" << '\n';
    return 0;
}