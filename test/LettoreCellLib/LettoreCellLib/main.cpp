#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include "../../../lib/Cell/cell.h"


int main()
{  
    std::ifstream inFile ("../../../files/InputFiles/Cell_Libraries/cell_library.ll", std::ios_base::out|std::ios_base::binary);
    if(!inFile)
        throw std::invalid_argument("file inesistente");

    std::ofstream outFile ("../../../files/InputFiles/Cell_Libraries/my_cell_library.ll");

    Cell c;
    std::vector<Cell> cells;
    while(inFile >> c)
    {
        if(c.getName() != "")
            cells.emplace_back(c);
        else
        {
            CellTimingInfo cinfo;
            Pin p;
            std::string aux;

            while(std::getline(inFile>>std::ws, aux) && aux.find('>') == std::string::npos);
            while((inFile >> cinfo && cinfo.getInfoType() != "") || (inFile >> p && p.name != ""));
        }

        c = Cell();
    }
    if(c.getName() != "")
        cells.emplace_back(c);

    for(auto& x : cells)
        outFile << x;
    std::cout << "all ok" << std::endl;

    return 0;
}

