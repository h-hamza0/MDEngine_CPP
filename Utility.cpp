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

void Utility::compute_velocity(Simulation &simulation){
    float dt = simulation.DT;
    float half_dt = dt / 2;
    float natoms = simulation.N_ATOMS;

    for (int i = 0; i < natoms; i ++){
        simulation.VELO[i][0] = simulation.VELO[i][0] + half_dt * simulation.ACCEL[i][0];
        simulation.VELO[i][1] = simulation.VELO[i][1] + half_dt * simulation.ACCEL[i][1];
        simulation.VELO[i][2] = simulation.VELO[i][2] + half_dt * simulation.ACCEL[i][2]; 
    };
};

void Utility::clear_force(Simulation &simulation){
    float natoms = simulation.N_ATOMS;
    for (int i = 0; i < natoms; i ++){
        simulation.FORCE[i] = {0, 0, 0};
    }
};

void Utility::find_acceleration(Simulation &simulation, float mass){
    float natoms = simulation.N_ATOMS;
    for (int i = 0; i < natoms; i++){
        simulation.ACCEL[i][0] = simulation.FORCE[i][0] / mass;
        simulation.ACCEL[i][1] = simulation.FORCE[i][1] / mass;
        simulation.ACCEL[i][2] = simulation.FORCE[i][2] / mass;
    }
};


ForceResult Utility::compute_force(Simulation &simulation){
    float n_atoms = simulation.N_ATOMS;
    float length = simulation.box_L;
    float base = simulation.box_B;
    float height = simulation.box_H;
    float half_L = length / 2;
    float half_B = base / 2;
    float half_H = height / 2;
    ForceResult ret;

    float sig12 = pow(0.34,12);
    float sig6 = pow(0.34, 6);
    float eps = 1.005841;
    float rcut = simulation.RCUT;
    float rcut2 = simulation.RCUT * simulation.RCUT;
    float pbc = simulation.PBC;
    float LJ1 = (6 * sig12 * pow(rcut, -12) - 3 * sig6 * pow(rcut, -6)) / rcut2;
    float LJ2 = -7*sig12*pow(rcut, -12) + 4*sig6*pow(rcut, -6);
    float LJ3 = (48*eps*sig12*pow(rcut, -12) - 24*eps*sig6*pow(rcut, -6)) / rcut2;
    float eww = 0;
    vector<int> virial = {0, 0, 0};

    for (int i = 0; i < n_atoms; i++){
        float xi = simulation.POST[i][0];
        float yi = simulation.POST[i][1];
        float zi = simulation.POST[i][2];

        for (int j = i + 1; j < n_atoms; j ++){
            float xj = simulation.POST[j][0];
            float yj = simulation.POST[j][1];
            float zj = simulation.POST[j][2];

            float xij = xi - xj;
            float yij = yi - yj;
            float zij = zi - zj; 
            
            if (pbc == 1){
                if (abs(zij) > half_H){
                    if (zij < 0){
                        zij += height;
                    }
                    else {
                        zij -= height;
                    }
                }
            }
            if (abs(xij) > half_L){
                if (xij < 0){
                    xij += length;
                }
                else {
                    xij -= length;
                }
            }

            if (abs(yij) > half_B){
                if (yij < 0){
                    yij += base;
                }
                else {
                    yij -= base;
                }
            }

            float rsqr = xij * xij + yij * yij + zij * zij;

            if (rsqr < rcut2){
                float irr = 1 / rsqr;
                float ir6 = irr * irr * irr;
                float ir12 = ir6 * ir6;
                float force_mag;
                eww = eww + 4*eps*(sig12*ir12 - sig6*ir6 + LJ1*rsqr + LJ2);
                force_mag = (48*eps*sig12*ir12 - 24*eps*sig6*ir6)*irr - LJ3;
                simulation.FORCE[i][0] = simulation.FORCE[i][0] + force_mag * xij;
                simulation.FORCE[i][1] = simulation.FORCE[i][1] + force_mag * yij;
                simulation.FORCE[i][2] = simulation.FORCE[i][2] + force_mag * zij;

                simulation.FORCE[j][0] = simulation.FORCE[j][0] - force_mag * xij;
                simulation.FORCE[j][1] = simulation.FORCE[j][1] - force_mag * yij;
                simulation.FORCE[j][2] = simulation.FORCE[j][2] - force_mag * zij;

                virial[0] = virial[0] - 0.5 * force_mag * xij * xij;
                virial[1] = virial[1] - 0.5 * force_mag * yij * yij;
                virial[2] = virial[2] - 0.5 * force_mag * zij * zij;
            }
        }
    }
    ret.accumulation = eww/n_atoms;
    ret.potential = 0;
    ret.virial = virial;
    return ret;
};