#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../Cell/cell.h"
#include <regex>
#include <memory>
#include <list>
#include <iomanip>

//#define CIRCUIT_DEBUG

//#define CIRCUIT_DEBUG

class Circuit
{
    friend std::istream& operator>>(std::istream& is, Circuit& c);
    friend std::ostream& operator<<(std::ostream& os, const Circuit& c);

public:

    Circuit(std::vector<std::vector<Cell>>* selection);

    double GetWorstCase();
    void AssignRandom();

private:
    std::list<std::pair<Cell, std::vector<Cell*>>> adjList;
    std::vector<std::vector<Cell>>* cellSelection;

    bool readInstruction1(std::string line);
    bool readInstruction2(std::string line);
    bool readInstruction3(std::string line);

    std::list<std::pair<Cell, std::vector<Cell*>>>::iterator searchOutputSingal(std::string name);
    std::vector<std::vector<Cell>>::iterator searchCellType(std::string name);

};

#endif // CIRCUIT_H
