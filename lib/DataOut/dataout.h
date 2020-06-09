#ifndef DATAOUT_H
#define DATAOUT_H
#include <string>
#include "../Circuit/circuit.h"
#include <iostream>
#include <fstream>

class CircuitOut
{

public:
    CircuitOut(const Circuit& c):c(c){};
    std::vector<double>& operator=(const std::vector<double>& DNA);
    std::pair<std::vector<double>, double> operator=(std::pair<std::vector<double>, double>& o);

    Circuit GetCircuit();
    std::vector<double> GetDNA();
    bool IsTraking() const {return traking;}

private:
    std::vector<double> DNA;
    double performance;
    const Circuit& c;
    bool traking;
};

#endif // DATAOUT_H
