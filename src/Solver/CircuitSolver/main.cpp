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


struct Args
{
    Args(){}
    Args(std::string cp, std::string clp, std::string op, BRKGAParams p,
         size_t mg, size_t dl, double dg, bool tt, CriteriaPolicy py, bool po, double bias)
        : circuitPath(cp), celllibPath(clp), outFilePath(op), p(p),
          maxgens(mg), deadlock(dl), delaygoal(dg), bias (bias), timeTrack(tt), policy(py),
          plotOutput(po)
    {}
    std::string circuitPath;
    std::string celllibPath;
    std::string outFilePath;
    BRKGAParams p;
    size_t maxgens;
    size_t deadlock;
    double delaygoal;
    double bias;
    bool timeTrack;
    CriteriaPolicy policy;
    bool plotOutput;
};


void ThreadSolver(Args a);
std::vector<std::vector<Cell>> readCellLibrary(const std::string& filename);

enum class paolo {bello, come, stai};


int main(int argc, char *argv[])
{
    srand(time(nullptr));
    int opt;


    const char *shortOpts = ":c:l:f:p:e:m:r:g:t:d:aib:n";
    const int minOpts = 14;

    Args a ("../../../files/InputFiles/Circuits/adder64.vhdl",
            "../../../files/InputFiles/Cell_Libraries/my_cell_library.hs",
            "", BRKGAParams(20, 0, 2, 3, 0.7),
            0, 0, 0, 1, CriteriaPolicy::OrPolicy, 0, -1);
    if(argc > 1)
    {
        while((opt = getopt(argc, argv, shortOpts)) != -1)
        {
            switch(opt)
            {

            case 'c':
                a.circuitPath = optarg;
                break;
            case 'l':
                a.celllibPath = optarg;
                break;
            case 'f':
                a.outFilePath = optarg;
                if(a.outFilePath == ".")
                    a.outFilePath = "";
                break;
            case 'p':
                a.p.popSize = std::stoi(optarg);
                break;
            case 'e':
                a.p.pe = std::stoi(optarg);
                break;
            case 'm':
                a.p.pm = std::stoi(optarg);
                break;
            case 'r':
                a.p.rho_e = std::stod(optarg);
                break;
            case 'g':
                a.maxgens = std::stoul(optarg);
                break;
            case 'd':
                a.deadlock = std::stoul(optarg);
                break;
            case 't':
                a.delaygoal = std::stod(optarg);
                break;
            case 'a':
                a.policy = CriteriaPolicy::AndPolicy;
                break;
            case 'i':
                a.timeTrack = 0;
                break;
            case 'n':
                a.plotOutput = 1;
                break;
            case 'b':
                //std::cout << "-b: "<< optarg << std::endl;
                a.bias = std::stod(optarg);
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
        std::cout << "circuit path: " << a.circuitPath << std::endl;
        std::cout << "celllib path: " << a.celllibPath << std::endl;
        std::cout << "outFile path: " << a.outFilePath << std::endl;
        std::cout << a.p << std::endl;
    }
    else
    {
        a.maxgens = 10;
        a.deadlock = 5;
        //a.delaygoal = 3.52;
        a.policy = CriteriaPolicy::AndPolicy;
    }

    // ***************************** Lettura files **********************************
    ThreadSolver(a);


    return 0;
}



void ThreadSolver(Args a)
{
    std::vector<std::vector<Cell>> cells = readCellLibrary(a.celllibPath);
    std::cout << "cells initilized" << std::endl;

    std::ifstream circuitFile(a.circuitPath, std::ios_base::in|std::ios_base::binary);
    if(!circuitFile)
    {
        throw std::invalid_argument("file del circuito inesistente");
    }
    Circuit c(&cells);
    circuitFile >> c;
    circuitFile.close();
    std::cout << "circuit initialized" << std::endl;

    // ******************************************************************************

    if(a.maxgens + a.delaygoal + a.deadlock == 0.0)
        std::cout << "WARNING: Algorithm won't stop" << std::endl;


    CircuitGraph ss(c);
    c.AssignRandom();
    ss.SetupCaps();
    std::cout << "Delay: " << ss.getWorstPathDistance() << " Area: " << ss.GetAreaOccupation() << " " << ss.GetNumOfNodes() << std::endl;

    c.AssignAll(0);
    ss.SetupCaps();
    std::cout << "Delay: " << ss.getWorstPathDistance() << " Area: " << ss.GetAreaOccupation() << " " << ss.GetNumOfNodes() << std::endl;


    c.AssignAll(1);
    ss.SetupCaps();
    std::cout << "Delay: " << ss.getWorstPathDistance() << " Area: " << ss.GetAreaOccupation() << " " << ss.GetNumOfNodes() << std::endl;


    a.p.nAlleles = c.GetNumOfCells();
    a.p.UpdateFromPop();

    CircuitSolver s(c, a.p);

//    std::cout << a.maxgens << " " << a.delaygoal << " " << a.deadlock << std::endl;

    s.setBias(a.bias);
    //std::cout << "bias: " << a.bias << std::endl;
    s.setDeadlock(a.deadlock);
    s.setMaxGens(a.maxgens);
    s.setDelayGoal(a.delaygoal);
    s.setCriteriaPolicy(a.policy);
    s.TrackEvolution(1, "", a.timeTrack);
    if(a.outFilePath == "")
        std::cout << "---" << std::endl;
    s.Evolve();
    if(a.outFilePath == "")
        std::cout << "---" << std::endl;

    if(a.plotOutput)
    {
        std::ofstream out("../../../files/output-adiacenza.txt", std::ios_base::out|std::ios_base::binary);
        out << c;
        out.close();
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


