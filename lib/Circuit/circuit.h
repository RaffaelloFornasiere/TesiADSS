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

    size_t GetNumOfCells() const {return adjList.size();}
    const Cell* GetCell(int i) const {return inputLists[i].first;}
    std::vector<const Cell*> GetInputsOfCell(int cell) const {std::vector<const Cell*> aux; for(auto i : inputLists[cell].second)aux.push_back(i); return aux;}
    std::vector<const Cell*> GetInputsOfCell(const Cell* cell) const;
    std::vector<const Cell*> GetOutputsOfCell(int cell) const;
    std::vector<const Cell*> GetOutputsOfCell(const Cell* c) const;

    void ChangeCell(const Cell *c) const;

private:
    mutable std::list<std::pair<Cell, std::vector<Cell*>>> adjList;
    std::vector<std::pair<Cell*, std::vector<Cell*>>> inputLists;

    std::vector<std::vector<Cell>>* cellSelection;

    bool readInstruction1(std::string line);
    bool readInstruction2(std::string line);
    bool readInstruction3(std::string line);

    std::list<std::pair<Cell, std::vector<Cell*>>>::iterator searchOutputSingal(std::string name);
    std::vector<std::vector<Cell>>::iterator searchCellType(std::string name);

};

#endif // CIRCUIT_H
