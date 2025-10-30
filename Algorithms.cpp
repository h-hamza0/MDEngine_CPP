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

void Algorithms::velocity_scaling(Simulation &simulation, float t_bulk){
    float scale;
    if (simulation.THERMOSTAT == 1){
        scale = sqrt(1 + simulation.DT *((simulation.T_REF/t_bulk) - 1/ simulation.TAU));
    }
    if (simulation.THERMOSTAT == 2){
        scale = sqrt(simulation.T_REF / t_bulk);
    }
    int n_atoms = simulation.N_ATOMS;
    for (int i = 0; i < n_atoms; i ++){
        simulation.VELO[i][0] = simulation.VELO[i][0] * scale;
        simulation.VELO[i][1] = simulation.VELO[i][1] * scale;
        simulation.VELO[i][2] = simulation.VELO[i][2] * scale;
    }
};