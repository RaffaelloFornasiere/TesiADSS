#include "circuitsolvernsga.h"


void CircuitSolverNSGA::Decode(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end)
{
    for(auto& i = begin; i != end; ++i)
    {
        output.SetAllCells((*i).dna);
        output.SetupCaps();

        i->objectives[0] = output.getWorstPathDistance();
        i->objectives[1] = output.GetAreaOccupation();
    }
}

bool CircuitSolverNSGA::StopCriteria()
{
    if(tracking)
    {
        // pareto front
        std::string print;
        for(size_t i = 0; i < ranks[0].size(); i++)
        {
            for(auto& o: ind[i].objectives)
                print += std::to_string(o) + " ";
           // print += '\n';
        }

        std::cout << print  << "\n" << std::flush;
    }
    return p.maxGenerations >= generations;
}
