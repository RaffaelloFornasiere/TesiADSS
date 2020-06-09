#include "dataout.h"


std::vector<double>& CircuitOut::operator=(const std::vector<double>& DNA)
{
    this->DNA = DNA;
    return this->DNA;
}

std::pair<std::vector<double>, double> CircuitOut::operator=(std::pair<std::vector<double>, double> &o)
{
    DNA = o.first;
    performance = o.second;
    return std::make_pair(DNA, performance);
}
