#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void read_input();

class Simulation{
public:
    void read_input(std::string);
private:
    float DT;
    float TOT_STEPS;
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

