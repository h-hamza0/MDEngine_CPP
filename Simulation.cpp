#include "Simulation.h"
#include "Algorithms.h"
#include "Output.h"
#include "Utility.h"

void Simulation::integrate(Algorithms* &algos, Utility &util, int &PRESS_COUNT, int &i, ForceResult &fc, float &mass, KineticResult &ke, Output* &out, float &box_VOL){
    bool pressure = false;
    algos -> velocity_verlet(*this);
    util.check_and_update(*this);
    util.compute_velocity(*this);
    if (i % 50 == 0){
        pressure = true;
        PRESS_COUNT += 1; 
    };
    util.clear_force(*this);
    fc = util.compute_force(*this);
    float eww = fc.accumulation;
    float ewpt = fc.potential;
    vector<float> virial = fc.virial;
    util.find_acceleration(*this, mass);
    util.compute_velocity(*this);
    if (std::fmod(i, this->NST_COMM) == 0){
        util.com_removal(*this);
    };
    ke = util.find_ke_temp(*this, mass);
    float t_bulk = ke.T_BULK;
    vector<float> kin = ke.kinetic;
    float KE = ke.KE;
    if (this->THERMOSTAT != 0){
        algos -> velocity_scaling(*this, t_bulk);
    }
    if (std::fmod(i, this->PRESS_INTVL) == 0){
        out -> write_virial(virial, kin, box_VOL, i);
    };
    float e_total = eww + ewpt;
    this -> E_TOTAL = e_total;
    this -> EWW = eww;
    this -> T_BULK = t_bulk;
    out -> write_res(KE, e_total, t_bulk, i);
    if (std::fmod(i, this->nstxout) == 0){
        out -> write_xyz(*this, i);
    }
};
void Simulation::read_input(std::string f){
    std::ifstream file(f); // opens file

    if (!file.is_open()) {
        std::cerr << "Could not open file!\n"; 
        std::exit(EXIT_FAILURE);
    };
    std::string line; 

    while (std::getline(file, line)) { 
        std::istringstream iss(line); 
        std::vector<std::string> tokens;
        std::string word;
        std::string varType;
        float val;

        while (iss >> word) {
            tokens.push_back(word);
        }
        if (!tokens.empty()){
            if (tokens[0][0] != ';'){
                varType = tokens[0];
                val = std::stof(tokens[2]);
                if (varType == "dt"){
                    DT = val;
                } else if (varType == "nsteps") {
                    TOT_STEPS = val;
                } else if (varType == "PRESS_INTVL") {
                    PRESS_INTVL = val;
                } else if (varType == "SLAB_THICK") {
                    SLAB_THICK = val;
                } else if (varType == "THERMOSTAT") {
                    cout << val << endl;
                    THERMOSTAT = val;
                } else if (varType == "T_REF") {
                    T_REF = val;
                } else if (varType == "sigma_ARPT") {
                    sigma_ARPT = val;
                } else if (varType == "eps_ARPT") {
                    eps_ARPT = val;
                } else if (varType == "rc_wall") {
                    rc_wall = val;
                } else if (varType == "RCRIT") {
                    RCRIT = val;
                } else if (varType == "INIT_MOMENT") {
                    INIT_MOMENT = val;
                } else if (varType == "RCUT") {
                    RCUT = val;
                } else if (varType == "NST_COMM") {
                    NST_COMM = val;
                } else if (varType == "nstxout") {
                    nstxout = val;
                } else if (varType == "PBC") {
                    PBC = val;
                } else if (varType == "WALL_MODEL") {
                    WALL_MODEL = val;
                } else if (varType == "top_w") {
                    top_w = val;
                } else if (varType == "bot_w") {
                    bot_w = val;
                } else if (varType == "TAU") {
                    TAU = val;
                } else {
                    cout << "ERROR: INPUT FILE ERROR\n";
                }
            }
        }
        }
    };

void Simulation::allot_memory(int ARG) {
    for (int i = 0; i < ARG; i++) {
        ACCEL.push_back({0, 0, 0});
        FORCE.push_back({0, 0, 0});
    }
};

void Simulation::read_system(std::string f){
    std::ifstream file(f);
    cout << "File in argon.csv" << endl;
    bool firstLine = true;
    if (!file.is_open()) {
        std::cerr << "FILE COULD NOT BE OPENED\n";
        std::exit(EXIT_FAILURE);
    };

    std::string line;
    std::vector<std::vector<std::string>> data;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string val;
        std::vector<std::string> row;

        while (std::getline(ss, val, ',')) {
            row.push_back(val);
        }
        data.push_back(row);
    };
    file.close();
    N_ATOMS = std::stoi(data[0][0]);
    for (int i = 1; i < N_ATOMS + 1; i++) {
        std::vector<float> b, c;
        b.push_back(std::stof(data[i][2]));
        b.push_back(std::stof(data[i][3]));
        b.push_back(std::stof(data[i][4]));
        c.push_back(std::stof(data[i][5]));
        c.push_back(std::stof(data[i][6]));
        c.push_back(std::stof(data[i][7]));
        POST.push_back(b);
        VELO.push_back(c);
        
    }
    box_L = std::stof(data[data.size() - 1][0]);
    box_B = std::stof(data[data.size() - 1][1]);
    box_H = std::stof(data[data.size() - 1][2]);
    allot_memory(N_ATOMS);
};

void Simulation::displacement(){
    // method to compute the displacement of atoms
    float DT_squared = DT * DT;
    float DT_squared_half = 0.5 * DT_squared;

    for (int i = 0; i < N_ATOMS; i++){
        POST[i][0] = POST[i][0] + (DT * VELO[i][0]) + (DT_squared_half * ACCEL[i][0]);
        POST[i][1] = POST[i][1] + (DT * VELO[i][1]) + (DT_squared_half * ACCEL[i][1]);
        POST[i][2] = POST[i][2] + (DT * VELO[i][2]) + (DT_squared_half * ACCEL[i][2]);
    }
}