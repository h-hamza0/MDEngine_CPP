#include "Algorithms.h"

void Algorithms::velocity_verlet(Simulation &simulation){
    float DT = simulation.DT;
    float DT_SQR = DT * DT;
    float HALF_DT_SQR = 0.5 * DT_SQR;
    for (int i = 0; i < simulation.N_ATOMS; i ++){
        simulation.POST[i][0] = simulation.POST[i][0] + (DT * simulation.VELO[i][0]) + (HALF_DT_SQR * simulation.ACCEL[i][0]);
        simulation.POST[i][1] = simulation.POST[i][1] + (DT * simulation.VELO[i][1]) + (HALF_DT_SQR * simulation.ACCEL[i][1]);
        simulation.POST[i][2] = simulation.POST[i][2] + (DT * simulation.VELO[i][2]) + (HALF_DT_SQR * simulation.ACCEL[i][2]);
    }
};