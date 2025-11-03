#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../Simulation.h"
#include "../Utility.h"

TEST_CASE("Energy conservation over simulation"){
    Simulation sim;
    Algorithms* algos;
    Utility util;
    ForceResult fc;
    KineticResult ke;
    Output* out;
    sim.read_input("RUN_PARAMETERS.cfg");
    cout << "Read input" << endl;
    sim.read_system("argon.csv");
    cout << "Read system" << endl;
    float mass = 39.9;
    int PRESS_COUNT = 0;
    float E0;
    float box_VOL = sim.box_L * sim.box_B * sim.box_H;
    for (int i = 0; i < 1000; i ++) {
        cout << "Step " << i << " Energy = " << sim.E_TOTAL << endl;
        sim.integrate(algos, util, PRESS_COUNT, i, fc, mass, ke, out, box_VOL);
        if (i == 100){
            E0 = sim.E_TOTAL;
        };
    }
    double E1 = sim.E_TOTAL;
    REQUIRE(fabs(E1 - E0) < 0.2);
}