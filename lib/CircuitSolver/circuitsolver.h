#ifndef CIRCUITSOLVER_H
#define CIRCUITSOLVER_H
#include "../BRKGA/BRKGA.h"
#include "../CircuitGraphInterface/circuitgraphinterface.h"
//#include "../CircuitOutput/circuitoutput.h"
#include <fstream>
#include <iostream>
#include <cmath>

typedef Circuit CircuitInput;
typedef CircuitGraph CircuitOutput;

enum CriteriaPolicy{OrPolicy, AndPolicy};


class CircuitSolver : public BRKGA<CircuitInput, CircuitOutput, double>
{
public:

    CircuitSolver(const CircuitInput& input, const  BRKGAParams& params, int init = 0)
        : BRKGA<CircuitInput, CircuitOutput, double>(input, params), output(input, init)
    {
        deadlock = 0;
        maxGens = 0;
        fitGoal = 0;
        fitnessExp = 3;
        counter = 0;
        lastBest = 0;
        k = 100;
    };
    ~CircuitSolver() override {if(dataOut.is_open())dataOut.close();}

    // *********** Solver methods **************
    void Decode() override;
    bool StopCriteria() override;
    double BestFitness() const override;
    // *****************************************

    void TrackEvolution(bool track, const std::string& fileName);
    void TrackEvolution(bool track, const std::string& fileName, size_t mutexIndex);

    size_t getDeadlock() const;
    void setDeadlock(size_t value);

    size_t getMaxGens() const;
    void setMaxGens(const size_t &value);

    double getDelayGoal() const;
    void setDelayGoal(double value);

    CriteriaPolicy getCriteriaPolicy() const;
    void setCriteriaPolicy(const CriteriaPolicy &value);

    double getFitnessExp() const;
    void setFitnessExp(double value);

private:
    void _Reset() override;
    bool IsDeadlockCritReached();
    bool IsMaxGensCritReached();
    bool IsDelayCritReached();

    double f(double delay) const
    {
        //double res = pow(k/delay, fitnessExp);
        return pow(k/delay, fitnessExp);
    };
    double f_1(double fitness) const
    {
        return k/pow(fitness, 1.0/fitnessExp);
    };



// *********Stop Criterias**************
    size_t counter;
    double lastBest;
    size_t deadlock;

    size_t maxGens;

    double fitGoal;


    CriteriaPolicy criteriaPolicy;
// *************************************

    double fitnessExp, k;

    bool tracking;
    CircuitOutput output;
    std::ofstream dataOut;
};

#endif // CIRCUITSOLVER_H




