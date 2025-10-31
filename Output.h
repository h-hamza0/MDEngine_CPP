#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <variant>
#include <iostream>
#include <Simulation.h>

class Output{
public:
    void write_virial(vector<float> &virial, vector<float> &kinetic, float box_VOL, float STEP);
    void write_xyz(Simulation &simulation, float STEP);
    void write_res(float KE, float PE, float T, float STEP);
};