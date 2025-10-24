#include "F01_read_inputs.h"

void Simulation::read_input(std::string f){
    std::ifstream file(f); // opens file

    if (!file.is_open()) {
        std::cerr << "Could not open file!\n"; 
    };

    std::string line; 

    while (std::getline(file, line)) { 
        std::istringstream iss(line); 

        std::vector<std::string> tokens;
        std::string word;
        std::string varType; // when parsing in file
        float val; // parsing in file

        while (iss >> word) {
            tokens.push_back(word);
        }
        if (!tokens.empty()){
            for (std::string a : tokens){
                if (a[0] != ';'){
                    varType = a[0];
                    val = float(a[2]);
                }
            }
        }
    };
};