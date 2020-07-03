#ifndef CIRCUITSOLVER_H
#define CIRCUITSOLVER_H
#include "../BRKGA/BRKGA.h"
#include "../CircuitGraphInterface/circuitgraphinterface.h"
#include <fstream>
#include <iostream>
#include <cmath>

typedef Circuit CircuitInput;
typedef CircuitGraph CircuitOutput;

enum CriteriaPolicy{OrPolicy, AndPolicy};


class CircuitSolver : public BRKGA<CircuitInput, CircuitOutput, double>
{
public:
    CircuitSolver(const CircuitInput& input, const  BRKGAParams& params);
    ~CircuitSolver() override {if(dataOut.is_open())dataOut.close();}

    double BestFitness() const override;

    void TrackEvolution(bool track, const std::string& fileName, bool timeTracking = 1);
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
    // *********** Solver methods **************
    void Decode() override;
    bool StopCriteria() override;
    void _Reset() override;
    // *****************************************

    bool IsDeadlockCritReached();
    bool IsMaxGensCritReached();
    bool IsDelayCritReached();

    // *********Stop Criterias**************
    size_t counter;
    double lastBest;
    size_t deadlock;

    size_t maxGens;
    double fitGoal;
    CriteriaPolicy criteriaPolicy;
    // *************************************

    double fitnessExp, k;
    double f(double delay) const
    {
        return pow(k/delay, fitnessExp);
    }
    double f_1(double fitness) const
    {
        return k/pow(fitness, 1.0/fitnessExp);
    }

    bool tracking;
    bool timeTracking;
    time_t lastTimeRec;

    std::ofstream dataOut;
};

#endif // CIRCUITSOLVER_H




