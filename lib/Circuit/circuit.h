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
//  that contains the real cells used to form the circuit.
//  There a matrix (cell selection) that contains all the cells availables to choose
//  from when building the circuit.
//  It also has a rendoundant array that for each cells contains the
//  cells that are connected in input. A sort of reverse adjacency list that
//  helps to reduce the time complexity while searching cells.
//  In attition to the common setters and getters it provides a method to
//  chance a specific cell, there are 2 implementations available:
//  - random: the cell is selected randomly among the cells of the same type
//  - according to a value p: in input is given a value from 0 to 1 that will be
//  converted into a number that represent the i entry of the vector that stores
//  the cells of the same type of the one received as input
//  *****************************************************************************

class Circuit
{
    friend std::istream& operator>>(std::istream& is, Circuit& c);
    friend std::ostream& operator<<(std::ostream& os, const Circuit& c);
    friend bool operator==(const Circuit& c1, const Circuit& c2);
    friend bool operator!=(const Circuit& c1, const Circuit& c2) {return !(c1 == c2);}


public:

    Circuit(const std::vector<std::vector<Cell>> *selection)
        : cellSelection(selection)
    {}

    Circuit(const std::vector<std::vector<Cell>> *selection, std::istream& is)
        : cellSelection(selection)
    {
        is >> *this;
    }

    double GetWorstCase();
    void AssignRandom();
    void AssignAll(double p) const;

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

    double GetAreaOccupation() const;


private:
    mutable std::list<std::pair<Cell, std::vector<Cell*>>> adjList;
    std::vector<std::pair<Cell*, std::vector<Cell*>>> inputLists;

    const std::vector<std::vector<Cell>>* cellSelection;



    bool readInstruction1(std::string line);
    bool readInstruction2(std::string line);
    bool readInstruction3(std::string line);

    std::list<std::pair<Cell, std::vector<Cell*>>>::iterator searchOutputSingal(std::string name);
    std::vector<std::vector<Cell>>::const_iterator searchCellType(std::string name);

};

#endif // CIRCUIT_H
