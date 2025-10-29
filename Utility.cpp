#include <Utility.h>

void Utility::check_and_update(Simulation &simulation){
    float length = simulation.box_L;
    float base = simulation.box_B;
    float height = simulation.box_H;
    float pbc = simulation.PBC;

    for (int i = 0; i < simulation.N_ATOMS; i++){
        if (simulation.POST[i][0] < 0){
            simulation.POST[i][0] += length;
        };
        if (simulation.POST[i][0] > length){
            simulation.POST[i][0] -= length;
        };
        if (simulation.POST[i][1] < 0){
            simulation.POST[i][1] += base;
        }
        if (simulation.POST[i][1] > length){
            simulation.POST[i][1] -= base;
        }
    }

    if (simulation.PBC == 1){
        for (int i = 0; i < simulation.N_ATOMS; i++){
            if (simulation.POST[i][2] < 0){
                simulation.POST[i][2] += height;
            };
            if (simulation.POST[i][2] > height){
                simulation.POST[i][2] -= height;
            }
        }
    }
};

void Utility::com_removal(Simulation &simulation){
    float vxsum, vysum, vzsum = 0;
    int natoms = simulation.N_ATOMS;
    for (int i = 0; i < natoms; i ++){
        vxsum += simulation.VELO[i][0];
        vysum += simulation.VELO[i][1];
        vzsum += simulation.VELO[i][2];
    }
    vxsum, vysum, vzsum /= natoms;

    for (int i = 0; i < natoms; i ++){
        simulation.VELO[i][0] -= vxsum;
        simulation.VELO[i][1] -= vysum;
    }

    if (simulation.PBC == 1){
        for (int i = 0; i < natoms; i ++){
            simulation.VELO[i][2] -= vzsum;
        }
    }
};


KineticResult Utility::find_ke_temp(Simulation &simulation, int mass){
    KineticResult result;
    float ke_x, ke_y, ke_z = 0;
    float boltz = 0.008314462175;
    float natoms = simulation.N_ATOMS;
    float ke = 0;
    for (int i = 0; i < natoms; i++){
        float vxx = simulation.VELO[i][0] * simulation.VELO[i][0];
        float vyy = simulation.VELO[i][1] * simulation.VELO[i][1];
        float vzz = simulation.VELO[i][2] * simulation.VELO[i][2];
        ke += (vxx + vyy + vzz);
        ke_x += (0.5 * vxx * mass);
        ke_y += (0.5 * vyy * mass);
        ke_z += (0.5 * vzz * mass);
    }
    ke = ke * 0.5 * mass;
    float t_bulk = ke * 2 / (3 * natoms * boltz);
    ke /= natoms;
    vector<float> kinetic = {ke_x, ke_y, ke_z};
    result.T_BULK = t_bulk;
    result.KE = ke;
    result.kinetic = kinetic;
    return result;
};