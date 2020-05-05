#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../../../lib/Circuit/circuit.h"
#include <queue>
#include <time.h>

int main()
{
    // -----------------------------------------------------------------------------------------------------------------------------------
    //                                                              SETUP
    // -----------------------------------------------------------------------------------------------------------------------------------

    srand(time(nullptr));
    std::ifstream inFile ("../../InputFiles/my_cell_library.txt", std::ios_base::out|std::ios_base::binary);
    if(!inFile)
        throw std::invalid_argument("file librerie inesistente");

    std::ifstream inFile2 ("../InputFiles/rc.029.vhdl", std::ios_base::out|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");

    std::ofstream outFile("../../GraphViewer/GraphViewer/output-adiacenza.txt", std::ios_base::out|std::ios_base::binary);

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

    std::cout << sizeof (cells) <<std::endl;

    Circuit c(&toPass);
    inFile2 >> c;
    outFile << c;

    return 0;
}


