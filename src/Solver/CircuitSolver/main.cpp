#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <iomanip>
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


void PopulationTuner(BRKGAParams p, std::vector<std::vector<Cell>>* selection, size_t maxPopSize, size_t step)
{
    std::ifstream inFile2 ("../../../files/InputFiles/Circuits/rc.001.vhdl", std::ios_base::in|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");

    std::ofstream outFileGraph("../../../files/data-output.txt", std::ios_base::out|std::ios_base::binary);

    Circuit c(selection);
    inFile2 >> c;
    inFile2.close();

    size_t ip = p.popSize;
    CircuitSolver s(&c, p, 0);
    time_t start;
    for(size_t pop = ip; pop < maxPopSize; pop += step)
    {
        p.popSize = pop;
        p.pe = pop*p.ppe;
        p.pm = pop*p.ppm;

        std::cerr << std::setw(5) << p.popSize << " "
                  << std::setw(5) << p.pe << " "
                  << std::setw(5) << p.pm << " "
                  << std::setw(8) << p.rho_e << " ";
        s.Reset();
        s.SetParams(p);
        start = clock();
        s.Evolve();
        std::cerr << std::setw(10) << s.BestSolution() << std::setw(5) << " " << clock()-start << std::endl;
    }

}



void EliteMutantTuner(BRKGAParams p, std::string fileDir, std::vector<std::vector<Cell>>* selection, double finalPpe, double finalPpm, double step)
{
    std::ifstream inFile2 (fileDir, std::ios_base::in|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");

    std::ofstream outFileGraph("../../../files/data-output.txt", std::ios_base::out|std::ios_base::binary);

    Circuit c(selection);
    inFile2 >> c;
    inFile2.close();
    double ippm = p.ppm;
    CircuitSolver s(&c, p, 0);
    time_t start;
    while(p.ppe <= finalPpe)
    {
        p.pe = p.ppe*p.popSize;
        while(p.ppm <= finalPpm)
        {
            p.pm = p.ppm*p.popSize;
            std::cerr << std::setw(5) << p.popSize << " "
                      << std::setw(5) << p.pe << " "
                      << std::setw(5) << p.pm << " "
                      << std::setw(8) << p.rho_e << " ";
            s.Reset();
            s.SetParams(p);
            start = clock();
            s.Evolve();
            std::cerr << std::setw(10) << s.BestSolution() << std::setw(5) << " " << clock()-start << std::endl;
            //std::cout << std::endl;
            while(size_t(p.popSize*p.ppm) == size_t(p.popSize*(p.ppm+step)))
                p.ppm += step;
            p.ppm += step;
        }
        while(size_t(p.popSize*p.ppe) == size_t(p.popSize*(p.ppe+step)))
            p.ppe += step;
        p.ppe += step;

        p.ppm = ippm;
    }

}

void RhoeTuner(BRKGAParams p, std::string fileDir, std::string outFile, std::vector<std::vector<Cell>>* selection, double finalRho_e, double step)
{
    std::ifstream inFile2 (fileDir, std::ios_base::in|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");

    std::ofstream dataOut(outFile, std::ios_base::out|std::ios_base::binary);

    Circuit c(selection);
    inFile2 >> c;
    inFile2.close();

    //double ip = p.rho_e;
    time_t start;
    CircuitSolver s(&c, p, 0);
    while(p.rho_e < finalRho_e)
    {
        //EliteMutantTuner(p, fileDir, selection, 0.25, 0.3, 0.01);
        dataOut << std::setw(5) << p.popSize << " "
                  << std::setw(5) << p.pe << " "
                  << std::setw(5) << p.pm << " "
                  << std::setw(8) << p.rho_e << " ";
        s.Reset();
        s.SetParams(p);
        start = clock();
        s.Evolve();
        dataOut << std::setw(10) << s.BestSolution() << std::setw(5) << " " << clock()-start << std::endl;
        //std::cout << p.rho_e << std::endl;
        p.rho_e += step;
    }

}




int main()
{

    // -----------------------------------------------------------------------------------------------------------------------------------
    //                                                              SETUP
    // -----------------------------------------------------------------------------------------------------------------------------------
    time_t start = clock();
    srand(1);

    std::string fileDir ("../../../files/InputFiles/Circuits/rc.029.vhdl");
    std::ifstream inFile2 (fileDir, std::ios_base::in|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");
    std::ofstream outFile("../../../files/output-adiacenza.txt", std::ios_base::out|std::ios_base::binary);
    std::vector<std::vector<Cell>> toPass = readCellLibrary(std::string("../../../files/InputFiles/Cell_Libraries/my_cell_library.hs"));

    // *********************************************************************************
    //                          Reads the circuit
    // *********************************************************************************
    Circuit c(&toPass);
    inFile2 >> c;
    inFile2.close();

    std::vector<std::thread> threads;
    {
        std::string outDir("../../../files/rhoe/pe01pm01.txt");
        double ppe = 0.1;
        double ppm = 0.1;
        double rho_e = 0.5;
        double pop = 100;
        BRKGAParams p (pop, c.GetNumOfCells(), ppe*pop, ppm*pop, rho_e, 0);
        threads.push_back(std::thread(RhoeTuner, p, fileDir, outDir, &toPass, 0.8, 0.03));
    }
    {
        std::string outDir("../../../files/rhoe/pe02pm03.txt");
        double ppe = 0.2;
        double ppm = 0.3;
        double rho_e = 0.5;
        double pop = 100;
        BRKGAParams p (pop, c.GetNumOfCells(), ppe*pop, ppm*pop, rho_e, 0);
        threads.push_back(std::thread(RhoeTuner, p, fileDir, outDir, &toPass, 0.8, 0.03));
    }









    for(auto& i : threads)
        i.join();
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
