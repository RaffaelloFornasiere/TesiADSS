#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include "../../../lib/CircuitSolverNSGA/circuitsolvernsga.h"

std::vector<std::vector<Cell>> readCellLibrary(const std::string& filename);


int main(int argc, char *argv[])
{
    argc++; argv[0] = argv[1];

    std::string circuitPath = "../../../files/InputFiles/Circuits/deco3.vhdl";
    std::string cellLibPath = "../../../files/InputFiles/Cell_Libraries/my_cell_library.hs";

    std::vector<std::vector<Cell>> cells = readCellLibrary(cellLibPath);
    std::cout << "cells initilized" << std::endl;

    std::ifstream circuitFile(circuitPath, std::ios_base::in|std::ios_base::binary);
    if(!circuitFile)
    {
        throw std::invalid_argument("file del circuito inesistente");
    }
    Circuit c(&cells);
    circuitFile >> c;
    circuitFile.close();
    std::cout << "circuit initialized" << std::endl;

    CircuitSolverNSGA solver(c, 30, 0.01, 50);
    solver.SetTracking(1);
    solver.InitializePopulation();
    solver.Evolve();


    return 0;
}



std::vector<std::vector<Cell>> readCellLibrary(const std::string& filename)
{
    std::ifstream inFile (filename, std::ios_base::in|std::ios_base::binary);
    if(!inFile)
        throw std::invalid_argument("file librerie inesistente");

    Cell auxCell;
    std::vector<Cell> cells;
    while(inFile >> auxCell)
    {
        if(auxCell.getName() != "")
            cells.emplace_back(auxCell);
        else
        {
            CellTimingInfo cinfo;
            Pin p;
            std::string aux;

            while(std::getline(inFile>>std::ws, aux) && aux.find('>') == std::string::npos);
            while((inFile >> cinfo && cinfo.getInfoType() != "") || (inFile >> p && p.name != ""));
        }

        auxCell = Cell();
    }
    if(auxCell.getName() != "")
        cells.emplace_back(auxCell);
    inFile.close();

    std::vector<std::vector<Cell>> toPass;
    std::sort(cells.begin(), cells.end(), [=](Cell& c1, Cell& c2){return c1.getType()<c2.getType();});
    toPass.push_back(std::vector<Cell>(1, cells.at(0)));

    for(size_t i = 1; i < cells.size(); i++)
    {
        if(cells.at(i).getType() == toPass.back().back().getType())
            toPass.back().push_back(cells.at(i));
        else
            toPass.push_back(std::vector<Cell>(1, cells.at(i)));
    }

    //toPass.front().front().TestTimingInfo();

    for(auto& i : toPass)
    {
        std::sort(i.begin(), i.end(), []
                  (const Cell& c1, const Cell& c2)
        {return c1.getArea() < c2.getArea();});
    }
    return toPass;
}

