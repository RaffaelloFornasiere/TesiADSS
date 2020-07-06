#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include "../../../lib/CircuitSolverNSGA/circuitsolvernsga.h"

std::vector<std::vector<Cell>> readCellLibrary(const std::string& filename);
struct SolverArgs
{
    std::string circuitPath;
    std::string cellLibPath;
    std::string dataPlotPath;

    size_t popSize;
    double mutationP;
    size_t maxGens;
};

struct A
{
    int a;
    int b;
};

int main(int argc, char *argv[])
{

    const char *shortOpts = ":c:l:p:m:g:";
    int opt;
    SolverArgs s;
    if(argc < 2)
    {
        s.circuitPath = "../../../files/InputFiles/Circuits/deco3.vhdl";
        s.cellLibPath = "../../../files/InputFiles/Cell_Libraries/my_cell_library.hs";
        s.popSize = 30;
        s.mutationP = 0.01;
        s.maxGens = 50;
    }
    else
        while((opt = getopt(argc, argv, shortOpts)) != -1)
        {
            switch (opt)
            {
            case 'c':
                //std::cout << "circuit" << std::endl;
                s.circuitPath = optarg;
                break;
            case 'l':
                //std::cout << "cells" << std::endl;
                s.cellLibPath = optarg;
                break;
            case 'f':
                //std::cout << "dataplot" << std::endl;
                s.dataPlotPath = optarg;
                break;
            case 'p':
                //std::cout << "pop" << std::endl;
                s.popSize = std::stoi(optarg);
                break;
            case 'g':
                //std::cout << "maxgens" << std::endl;
                s.maxGens = std::stoi(optarg);
                break;
            case 'm':
                //std::cout << "mutation" << std::endl;
                s.mutationP = std::stod(optarg);
                break;
            case ':':
                std::cout << "missing argument" << std::endl;
                exit(EXIT_FAILURE);
            default:
                std::cout << "unexpected option" << std::endl;
                exit(EXIT_FAILURE);
            }
        }


    std::vector<std::vector<Cell>> cells = readCellLibrary(s.cellLibPath);
    //std::cout << "cells initilized" << std::endl;

    std::ifstream circuitFile(s.circuitPath, std::ios_base::in|std::ios_base::binary);
    if(!circuitFile)
    {
        throw std::invalid_argument("file del circuito inesistente");
    }
    //std::cout << s.circuitPath << std::endl;
    Circuit c(&cells);
    circuitFile >> c;
    circuitFile.close();
    //std::cout << "circuit initialized" << std::endl;

    CircuitSolverNSGA solver(c, s.popSize, s.mutationP, s.maxGens);
    solver.SetTracking(1);
    solver.InitializePopulation();
    solver.Evolve();

    if(s.dataPlotPath != "")
    {
        std::ofstream dataPlot(s.dataPlotPath, std::ios_base::in|std::ios_base::binary);
        dataPlot << c;
    }



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

