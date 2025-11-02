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
    Algorithms algos;
    Utility util;
    ForceResult fc;
    KineticResult ke;
    Output out;
    init.read_input("RUN_PARAMETERS.cfg");
    cout << "Read input" << endl;
    init.read_system("argon.csv");
    cout << "Read system" << endl;
    float mass = 39.9;
    int PRESS_COUNT = 0;
    float box_VOL = init.box_L * init.box_B * init.box_H;
    for (int i = 1; i < init.TOT_STEPS; i++){
        bool pressure = false;
        algos.velocity_verlet(init);
        util.check_and_update(init);
        util.compute_velocity(init);
        if (i % 50 == 0){
            pressure = true;
            PRESS_COUNT += 1; 
        };
        util.clear_force(init);
        fc = util.compute_force(init);
        float eww = fc.accumulation;
        float ewpt = fc.potential;
        vector<float> virial = fc.virial;
        util.find_acceleration(init, mass);
        util.compute_velocity(init);
        if (std::fmod(i, init.NST_COMM) == 0){
            util.com_removal(init);
        };
        ke = util.find_ke_temp(init, mass);
        float t_bulk = ke.T_BULK;
        vector<float> kin = ke.kinetic;
        float KE = ke.KE;
        if (init.THERMOSTAT != 0){
            algos.velocity_scaling(init, t_bulk);
        }
        if (std::fmod(i, init.PRESS_INTVL) == 0){
            out.write_virial(virial, kin, box_VOL, i);
        };
        float e_total = eww + ewpt;
        out.write_res(KE, e_total, t_bulk, i);
        if (std::fmod(i, init.nstxout) == 0){
        out.write_xyz(init, i);
        }
        cout << "Exec = " << (i * 100) / init.TOT_STEPS << " %, PE = " << eww << " T = " << t_bulk << endl;
    }
    cout << "End program" << '\n';
    return 0;
}