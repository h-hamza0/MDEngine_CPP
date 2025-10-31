#include <Output.h>

void Output::write_virial(vector<float> &virial, vector<float> &kinetic, float box_VOL, float STEP){
    if (STEP == 1) {
        std::ofstream f("Pressure.res");

        if (!f.is_open()){
            std::cerr << "Can not open file for writing";
        }
        f << "VirXX\tVirYY\tVirZZ\tPZZ\tPYY\tPZZ\tPavg\n";
    }
    else {
        std::ofstream f("Pressure.res", std::ios::app);
        if (!f.is_open()){
            std:cerr << "File is not readable";
        }
        float TOBAR = 16.6054;
        float PXX = 2 * TOBAR * (kinetic[0] - virial[0]) / box_VOL;
        float PYY = 2 * TOBAR * (kinetic[1] - virial[1]) / box_VOL;
        float PZZ = 2 * TOBAR * (kinetic[2] - virial[2]) / box_VOL;
        float P = (PXX + PYY + PZZ) / 3.0;

        f << virial[0] << '\t' << virial[1] << '\t' << virial[2] << '\t' << PXX << '\t' \
        << PYY << '\t' << PZZ << '\t' << P;
    }
};

void Output::write_xyz(Simulation &simulation, float STEP){
    float n_atoms = simulation.N_ATOMS;
    float dt = simulation.DT;
    float curr = dt * STEP;
    if (STEP == 1){
        std::ofstream f("traj_path.xyz");
        if (!f.is_open()) {
            std::cerr << "File unable to open!";
        }
    }
    else {
        std::ofstream f("traj_path.xyz", std::ios::app);
        f << n_atoms << std::endl;
        f << "@ " << curr << " ps" << std::endl;
    }
}