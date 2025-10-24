// Main enter program
#include <iostream>
#include <vector>
#include <variant>

#include "F01_read_inputs.h"
#include "F02_read_displacements.h"
#include "F03_common.h"
#include "F04_velo_accel.h"
#include "F05_find_force.h"
#include "F06_output.h"
// include other files below here
using namespace std;

using VarType = std::variant<float, vector<float>>;

// read inputs
ReadInputs init;
init.read_inputs();
init.read_argon();
// devise memory space for inputs

float mass = 39.9;
int PRESS_COUNT = 0;
float box_VOL = init.box_L * init.box_B * init.box_H;
int main() {
    for (int i = 1; i < init.TOT_STEPS; i++){
        bool pressure = false;
        disp.FIND_DISPLACEMENT();
        misc.CHECK_AND_UPDATE();
        va.FIND_VELOCITY();
        if (i % 50 == 0) {
            pressure = true; // pressure correction
            PRESS_COUNT += 1;
        }
        va.CLEAR_FORCE(); // clear force
        vector<VarType> RET = fo.FIND_FORCE();
        float EWW = RET[0];
        float EWPT = RET[1];
        vector<float> virial = RET[2];
        va.FIND_ACCELERATION(mass);
        ve.FIND_VELOCITY();
        if (i % init.NST_COMM == 0){
            misc.COM_REMOVAL();
        }
        vector<VarType> RET1 = misc.FIND_KE_TEMP(mass);
        float T_BULK = RET1[0];
        vector<float> kinetic = RET1[1];
        if (init.THERMOSTAT != 0) {
            misc.VELOCITY_SCALING(T_BULK);
        }
        if (i % init.PRESS_INVL == 0) {
            out.WRITE_VIRIAL(virial, kinetic, box_VOL, STEP_VAL);
        }
        float E_TOTAL = EWW + EWPT;
        out.WRITE_RES(KE, E_TOTAL, T_BULK, STEP_VAL);
        cout << 'Exec = ' << i * 100 / init.TOT_STEPS << " %,PE= " << EWW << "T = " << T_BULK << "\n";
        if (i % init.nstxout == 0) {
            out.WRITE_XYZ(i)
        }
    }
    cout << "End program" << '\n';
    return 0;
}