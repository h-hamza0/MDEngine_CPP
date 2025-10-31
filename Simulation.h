#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <cmath>
using namespace std;

class Simulation{
public:
    void read_input(std::string);
    void allot_memory(int);
    void read_system(std::string);
    void displacement();
    vector<vector<float>> ACCEL;
    vector<vector<float>> FORCE;
    vector<vector<float>> VELO;
    vector<vector<float>> POST;
    float box_L;
    float box_B;
    float box_H;
    int N_ATOMS;
    float TOT_STEPS;
    float DT;
    float PRESS_INTVL;
    float SLAB_THICK;
    float THERMOSTAT;
    float T_REF;
    float sigma_ARPT;
    float eps_ARPT;
    float rc_wall;
    float RCRIT;
    float INIT_MOMENT;
    float RCUT;
    float NST_COMM;
    float nstxout;
    float PBC;
    float WALL_MODEL;
    float top_w;
    float bot_w;
    float TAU;
};

class Memory {
public:
    void allot_memory(int ARG);
private:
    std::vector<std::array<double, 3>> ACCEL;
    std::vector<std::array<double, 3>> FORCE;
};

class Molecule {
public:
    void readMolecule();
private:
    std::vector<std::array<double, 3>> MOL;
    std::vector<std::array<double, 3>> VELO;
    int NMOL;
    int box_L;
    int box_B;
    int box_H;
};

struct KineticResult {
    float T_BULK;
    vector<float> kinetic;
    float KE;
};

struct ForceResult{
    float accumulation;
    float potential;
    vector<float> virial;
};