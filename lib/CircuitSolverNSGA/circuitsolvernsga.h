#ifndef CIRCUITSOLVERNSGA_H
#define CIRCUITSOLVERNSGA_H
#include "../NSGA-II/nsga_ii.h"
#include "../CircuitGraphInterface/circuitgraphinterface.h"
#include "../Circuit/circuit.h"



typedef Circuit CircuitInput;
typedef CircuitGraph CircuitOutput;

class CircuitSolverNSGA : public NSGA_II<Circuit, CircuitGraph>
{
public:
    CircuitSolverNSGA(const CircuitInput& input, size_t popSize, double mutationP = 0.01, size_t maxGenerations = 100)
        : NSGA_II<CircuitInput, CircuitOutput> (input, NSGAParams(popSize, input.GetNumOfCells(), 2, mutationP, maxGenerations)) {}
    ~CircuitSolverNSGA(){}

    void SetTracking(bool en){tracking = en;}

private:
    void _Reset() override {}
    void Decode(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end) override;
    bool StopCriteria() override;

    bool tracking;
};

#endif // CIRCUITSOLVERNSGA_H
