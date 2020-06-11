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
#include "../../../lib/CircuitSolver/circuitsolver.h"
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <signal.h>


std::vector<std::vector<Cell>> readCellLibrary(const std::string& filename)
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

    return toPass;
}


void PopulationTuner(std::string fileDir, std::string outFile, BRKGAParams p, std::vector<std::vector<Cell>>* selection, size_t maxPopSize, size_t step)
{
    std::ifstream inFile2 (fileDir, std::ios_base::in|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");

    std::ofstream dataOut;



    Circuit c(selection);
    inFile2 >> c;
    inFile2.close();

    size_t ip = p.popSize;
    CircuitSolver s(c, p, 0);
    time_t start;
    std::string path;
    for(size_t pop = ip; pop < maxPopSize; pop += step)
    {
        p.popSize = pop;
        p.UpdateFromRate();
        path = outFile + std::string("pop") + std::to_string(p.popSize) + std::string(".txt");
        dataOut.open(path, std::ios_base::out|std::ios_base::binary);


        dataOut << std::setw(5) << p.popSize << " "
                << std::setw(5) << p.pe << " "
                << std::setw(5) << p.pm << " "
                << std::setw(8) << p.rho_e << std::endl;

        s.Reset();
        s.SetParams(p);
        start = clock();
        s.TrackEvolution(1, path);
        s.Evolve();
        std::cout << std::setw(10) << s.BestFitness() << std::setw(5) << " " << clock()-start << std::endl;
        dataOut.close();

    }
    return;

}



void EliteMutantTuner(std::string fileDir, std::string outFile, BRKGAParams p, std::vector<std::vector<Cell>>* selection, double finalPpe, double finalPpm, double step)
{
    std::ifstream inFile2 (fileDir, std::ios_base::in|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");

    std::ofstream dataOut;

    Circuit c(selection);
    inFile2 >> c;
    inFile2.close();
    double ippm = p.ppm;
    CircuitSolver s(c, p, 0);
    time_t start;
    std::string path;
    while(p.ppe <= finalPpe)
    {
        while(p.ppm <= finalPpm)
        {
            std::cout << "\tbegin pepm tuning step" << std::endl;
            path = outFile + std::string("pepm") + std::to_string(int(p.ppe*100)) + std::to_string(int(100*p.ppm))+ std::string(".txt");
            dataOut.open(path, std::ios_base::out |std::ios_base::binary);

            dataOut << std::setw(5) << p.popSize << " "
                    << std::setw(5) << p.pe << " "
                    << std::setw(5) << p.pm << " "
                    << std::setw(8) << p.rho_e << std::endl;
            dataOut.close();

            s.Reset();
            s.SetParams(p);
            start = clock();
            s.TrackEvolution(1, path);
            s.setDeadlock(5);
            //s.SimpleTest();
            s.Evolve();
            dataOut.open(path, std::ios_base::out | std::ofstream::app | std::ios_base::binary);
            dataOut << "time: " << std::setw(5) << " " << clock()-start << std::endl;
            dataOut << "\tend pepm tuning step\n" << std::endl;
            dataOut.close();

            while(size_t(p.popSize*p.ppm) == size_t(p.popSize*(p.ppm+step)))
                p.ppm += step;
            p.ppm += step;
            p.UpdateFromRate();
        }
        while(size_t(p.popSize*p.ppe) == size_t(p.popSize*(p.ppe+step)))
            p.ppe += step;
        p.ppe += step;
        p.ppm = ippm;
        p.UpdateFromRate();
    }

}

void RhoeTuner(std::string fileDir, std::string outFile, BRKGAParams p,  std::vector<std::vector<Cell>>* selection, double finalRho_e, double step)
{
    srand(time(nullptr));
    std::ifstream inFile2 (fileDir, std::ios_base::in|std::ios_base::binary);
    if(!inFile2)
        throw std::invalid_argument("file del circuito inesistente2");

    std::ofstream dataOut;

    Circuit c(selection);
    inFile2 >> c;
    inFile2.close();

    //double ip = p.rho_e;
    time_t start;

    std::string path = outFile;
    CircuitSolver s(c, p, 0);
    while(p.rho_e <= finalRho_e)
    {

        std::cout << "\tbegin rhoe tuning step" << std::endl;
        path = outFile + std::string("rhoe") + std::to_string(int(p.rho_e*100)) + std::string(".txt");
        dataOut.open(path, std::ios_base::out|std::ios_base::binary);

        dataOut << std::setw(5) << p.popSize << " "
                << std::setw(5) << p.pe << " "
                << std::setw(5) << p.pm << " "
                << std::setw(8) << p.rho_e << std::endl;
        dataOut.close();


        s.Reset();
        s.SetParams(p);
        start = clock();
        s.TrackEvolution(1, path);
        s.setDeadlock(5);
        s.Evolve();

        dataOut.open(path, std::ios_base::out | std::ofstream::app | std::ios_base::binary);
        dataOut << "time: " << std::setw(5) << " " << clock()-start << std::endl;
        std::cout << "\tend rhoe tuning step\n" << std::endl;
        p.rho_e += step;
        dataOut.close();
    }

}



int main(int argc, char *argv[])
{
    srand(time(nullptr));
    int opt;


    const char *shortOpts = ":c:l:f:p:e:m:r:g:t:d:a";
    const int minOpts = 14;
    std::string circuitPath("../../../files/InputFiles/Circuits/adder16.vhdl");
    std::string celllibPath("../../../files/InputFiles/Cell_Libraries/my_cell_library.hs");
    std::string outFilePath;
    BRKGAParams p(100, 50, 20, 30, 0.7);
    size_t maxgens = 0;
    size_t deadlock = 0;
    double delaygoal = 0;
    CriteriaPolicy policy = CriteriaPolicy::OrPolicy;
    if(argc > 1)
    {
        while((opt = getopt(argc, argv, shortOpts)) != -1)
        {
            switch(opt)
            {
            case 'c':
                circuitPath = optarg;
                break;
            case 'l':
                celllibPath = optarg;
                break;
            case 'f':
                outFilePath = optarg;
                if(outFilePath == ".")
                    outFilePath = "";
                break;
            case 'p':
                p.popSize = std::stoi(optarg);
                break;
            case 'e':
                p.pe = std::stoi(optarg);
                break;
            case 'm':
                p.pm = std::stoi(optarg);
                break;
            case 'r':
                p.rho_e = std::stod(optarg);
                break;
            case 'g':
                maxgens = std::stoul(optarg);
                break;
            case 'd':
                deadlock = std::stoul(optarg);
                break;
            case 't':
                delaygoal = std::stod(optarg);
                break;
            case 'a':
                policy = CriteriaPolicy::AndPolicy;
                break;
            case ':':
                std::cerr << "Missing argument" << std::endl;
                return 1;
                break;
            default:
                std::cerr << "Unexpected option" << std::endl;
                return 2;
            }
        }
        if(optind < minOpts)
        {
            std::cerr << "Missing options" << std::endl;
            return 1;
        }

        std::cout << std::endl;
        std::cout << "circuit path: " << circuitPath << std::endl;
        std::cout << "celllib path: " << celllibPath << std::endl;
        std::cout << "outFile path: " << outFilePath << std::endl;
        std::cout << p << std::endl;
    }
    else
    {
        maxgens = 30;
        deadlock = 3;
        delaygoal = 3.52;
        policy = CriteriaPolicy::AndPolicy;
    }

    // ***************************** Lettura files **********************************
    std::vector<std::vector<Cell>> cells = readCellLibrary(celllibPath);
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
    // ******************************************************************************

    if(maxgens + delaygoal + deadlock == 0.0)
        std::cout << "WARNING: Algorithm won't stop" << std::endl;

    p.nAlleles = c.GetNumOfCells();
    p.UpdateFromPop();
    CircuitSolver s(c, p, 1);

    std::cout << maxgens << " " << delaygoal << " " << deadlock << std::endl;

    s.setDeadlock(deadlock);
    s.setMaxGens(maxgens);
    s.setDelayGoal(delaygoal);
    s.setCriteriaPolicy(policy);
    s.TrackEvolution(1, outFilePath);
    s.Evolve();
    //std::cout << "best solution" << s.BestFitness() << std::endl;


    std::cout << "fine main" << std::endl;
    return 0;
}










//int oldmain()
//{

//    srand(time(nullptr));
//    // -----------------------------------------------------------------------------------------------------------------------------------
//    //                                                              SETUP
//    // -----------------------------------------------------------------------------------------------------------------------------------
//    std::string fileDir ("../../../files/InputFiles/Circuits/adder64.vhdl");
//    std::ifstream inFile2 (fileDir, std::ios_base::in|std::ios_base::binary);
//    if(!inFile2)
//        throw std::invalid_argument("file del circuito inesistente2");
//    std::ofstream outFile("../../../files/output-adiacenza.txt", std::ios_base::out|std::ios_base::binary);
//    std::vector<std::vector<Cell>> toPass = readCellLibrary(std::string("../../../files/InputFiles/Cell_Libraries/my_cell_library.hs"));

//    // *********************************************************************************
//    //                          Reads the circuit
//    // *********************************************************************************
//    Circuit c(&toPass);
//    inFile2 >> c;
//    inFile2.close();
//    /*
//    double ppm = 0.3;
//    double ppe = 0.1;
//    size_t pop = 40;
//    BRKGAParams p (pop, c.GetNumOfCells(), pop*ppe, ppm*pop, 0.74);
//    CircuitSolver s(c, p, 1);
//    s.setDeadlock(3);
//    s.TrackEvolution(1, "../../../files/rhoe/test2.txt");
//    s.Evolve();

//*/
//    //5.72457







//    std::vector<std::thread> threads;
//    /*
//    {
//        std::string outDir = "../../../files/rhoe/";
//        double ppm = 0.3;
//        double ppe = 0.1;
//        size_t pop = 40;
//        BRKGAParams p (pop, c.GetNumOfCells(), pop*ppe, ppm*pop, 0.5);
//        //RhoeTuner(fileDir, outDir, p, &toPass, 0.8, 0.03);
//        threads.push_back(std::thread(RhoeTuner, fileDir, outDir, p, &toPass, 0.8, 0.03));
//    }
//*/
//    {
//        double ppm = 0.1;
//        double ppe = 0.1;
//        size_t pop = 40;
//        BRKGAParams p (pop, c.GetNumOfCells(), pop*ppe, ppm*pop, 0.7);
//        p.UpdateFromPop();
//        std::string outDir = "../../../files/pepm/";
//        threads.push_back(std::thread(EliteMutantTuner, fileDir, outDir, p, &toPass, 0.26, 0.2, 0.02));
//        p.ppm = 0.2;
//        p.UpdateFromRate();
//        threads.push_back(std::thread(EliteMutantTuner, fileDir, outDir, p, &toPass, 0.24, 0.3, 0.03));

//    }
//    /*   {
//        double ppm = 0.3;
//        double ppe = 0.1;
//        size_t pop = 20;
//        BRKGAParams p (pop, c.GetNumOfCells(), pop*ppe, ppm*pop, 0.7);
//        p.UpdateFromPop();
//        std::string outDir = "../../../files/pop/";
//        threads.push_back(std::thread(PopulationTuner, fileDir, outDir, p, &toPass, 110, 10));
//        p.popSize = 110;
//        p.UpdateFromRate();
//        threads.push_back(std::thread(PopulationTuner, fileDir, outDir, p, &toPass, 160, 10));
//        p.popSize = 160;
//        p.UpdateFromRate();
//        threads.push_back(std::thread(PopulationTuner, fileDir, outDir, p, &toPass, 200, 10));

//    }
//*/


//    for(auto& i : threads)
//        i.join();

//}
