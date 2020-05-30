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


//  *****************************************************************************
//  The class it's a input class for the circuit file.
//  Stores the topology of the circuit inside a adjacency list of cells
//  that contains the real cells used to deploy the circuit.
//  there a matrix that contains all the cells availables to choose from
//  in order to build the circuit
//  It also has a rendoundant array that for each cells contains the
//  cells that are connected in input. A sort of reverse adjacency list that
//  helps to reduce the time complexity.
//  In attition to the common setters and getters it provides a method to
//  randomly chance a specific cell: once it reads the type of the cell it
//  select randomly a new cell among ones of the same type.
//  *****************************************************************************

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

    std::vector<const Cell*> GetInputsOfCell(int cell) const;
    std::vector<const Cell*> GetInputsOfCell(const Cell* cell) const;

    std::vector<const Cell*> GetOutputsOfCell(int cell) const;
    std::vector<const Cell*> GetOutputsOfCell(const Cell* c) const;

    void ChangeCell(const Cell *c) const;
    bool ChangeCell(const Cell *c, double p) const;
    void ChangeCell(size_t i) const;
    bool ChangeCell(size_t i, double p) const;


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
