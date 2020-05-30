#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include "../../../lib/Cell/cell.h"


int main()
{  


    std::ifstream inFile ("../../../files/InputFiles/Cell_Libraries/cell_library.hs", std::ios_base::in|std::ios_base::binary);
    if(!inFile)
        throw std::invalid_argument("file inesistente");

    std::ofstream outFile ("../../../files/InputFiles/Cell_Libraries/my_cell_library.hs",  std::ios_base::out|std::ios_base::binary);

    Cell c;
    std::vector<Cell> cells;
    while(inFile >> c)
    {
        if(c.getName() != "")
        {
            cells.emplace_back(c);
            if ( c.getName().find("XOR3X4") != std::string::npos)
                std::cout << "aaa" << std::endl;
        }

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
    {
        std::cout << x.getName() << std::endl;
        outFile << x;
    }
    std::cout << "all ok" << std::endl;

    return 0;
}

