#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../../../lib/Circuit/circuit.h"
#include <queue>
#include <time.h>
#include "../../../lib/CircuitGraphInterface/circuitgraphinterface.h"



std::vector<std::vector<Cell>> readCellLibrary(std::string filename);

// ************************************************************************
// legge le celle
// legge il circuito
//  -   costruisce un grafo di celle. Viene salvato attraverso 2 liste di
//      adiacenza. Vengono usate due liste per ridurre il costo di ricerca
//  -   man mano che viene letto il circuito venogno create e collegate le
//      celle tra loro in funzione del nome del segnale al quale sono
//      collegate. Le celle create non hanno una corrispondenza reale.
//      questa verrà creata in seuito
// converte il circuito in un grafo
//  -   riceve in ingersso un puntatore al circuito e costruisce un grafo
//      che fa riferimento al circuito dove ogni coppia di nodi rappresenta
//      un collegamento. Ogni nodo rappresenta un pin di ingresso od un pin
//      di uscita ed ha dei parametri relativi alle dipendenze temporali
//      ed un parametro per la capacità che vede in uscita.
//      Per creare i nodi legge la lista di adiacenza e per ogni ingresso e
//      uscita di ogni cella crea un nodo del grafo. Collega poi tutti i nodi
//      e calcola le capacità. Infine viene ordinato il grafo in un array
//      tramite un ordinamento topologico.
// applica il risolutore
// ************************************************************************







int main()
{



    // -----------------------------------------------------------------------------------------------------------------------------------
    //                                                              SETUP
    // -----------------------------------------------------------------------------------------------------------------------------------
    time_t start = clock();
    srand(1);

    std::ifstream inFile2 ("../../../files/InputFiles/Circuits/rc.029.vhdl", std::ios_base::in|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");

    std::ofstream outFile("../../../files/output-adiacenza.txt", std::ios_base::out|std::ios_base::binary);

    std::vector<std::vector<Cell>> toPass = readCellLibrary(std::string("../../../files/InputFiles/Cell_Libraries/my_cell_library.hs"));

    // *********************************************************************************
    //                          Reads the circuit
    // *********************************************************************************
    Circuit c(&toPass);
    inFile2 >> c;
    //outFile << c; // only for graphing

    c.AssignAll(0.1);

    //std::cout << "Initialization done" << std::endl;
    size_t pop = 30;
    BRKGAParams p (pop, c.GetNumOfCells(), 0.2*pop, 0.3*pop, 0.6, 1);
    CircuitSolver var (&c, p, 1);

    size_t gens = 30;
    var.setMaxGenerations(gens);
    std::cout << "generations: " << gens << std::endl;
    std::cout << p << std::endl;
    var.Evolve();
    //std::cout << "delay: " << var.BestSolution() << std::endl;
    //std::cout << "area: " << var.GetAreaOccupation() << std::endl;


    std::cout << clock()-start << std::endl;
    return 0;
}


std::vector<std::vector<Cell>> readCellLibrary(std::string filename)
{
    //srand(time(nullptr));
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

    //toPass[0][0].TestTimingInfo();

    //std::cout << sizeof (cells) <<std::endl;
    return toPass;
}

