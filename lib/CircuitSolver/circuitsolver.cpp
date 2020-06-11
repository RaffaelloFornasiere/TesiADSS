#include "circuitsolver.h"


size_t CircuitSolver::getDeadlock() const
{
    return deadlock;
}

void CircuitSolver::setDeadlock(size_t value)
{
    deadlock = value;
}

size_t CircuitSolver::getMaxGens() const
{
    return maxGens;
}

void CircuitSolver::setMaxGens(const size_t &value)
{
    maxGens = value;
}

double CircuitSolver::getDelayGoal() const
{
    return f_1(fitGoal);
}

void CircuitSolver::setDelayGoal(double value)
{
    if(value == 0)
        fitGoal = 0;
    else
        fitGoal = f(value);
}


void CircuitSolver::_Reset()
{
    counter = 0;
    lastBest = 0;
}

bool CircuitSolver::IsDeadlockCritReached()
{
    if(fitVect.front().first > lastBest)
        counter++;
    else
    {
        counter = 0;
        lastBest = fitVect.front().first;
    }

    if(counter > deadlock)
        return 1;
    else
        return 0;
}

bool CircuitSolver::IsMaxGensCritReached()
{
    return generations >= maxGens;
}

bool CircuitSolver::IsDelayCritReached()
{
    return fitVect.front().first >= fitGoal;
}

double CircuitSolver::getFitnessExp() const
{
    return fitnessExp;
}

void CircuitSolver::setFitnessExp(double value)
{
    fitnessExp = value;
}

CriteriaPolicy CircuitSolver::getCriteriaPolicy() const
{
    return criteriaPolicy;
}

void CircuitSolver::setCriteriaPolicy(const CriteriaPolicy &value)
{
    criteriaPolicy = value;
}



void CircuitSolver::Decode()
{
    //for each element inside the fitvect i'll take

    for(auto& item : fitVect)
    {
        //time_t start = clock();
        std::vector<double> alleles = hypercube[item.second];

        output.SetAllCells(alleles);
        output.SetupCaps();

        item.first = f(output.getWorstPathDistance());

    }
}


bool CircuitSolver::StopCriteria()
{
    if(tracking)
    {
        if(dataOut.is_open())
            dataOut << f_1(fitVect.front().first) << std::endl;
        else
            std::cout << f_1(fitVect.front().first) << std::endl;
    }

    if(criteriaPolicy == CriteriaPolicy::OrPolicy)
        return  ((fitGoal == 0)?0:IsDelayCritReached())
                || ((maxGens == 0)?0:IsMaxGensCritReached())
                || ((deadlock == 0)?0:IsDeadlockCritReached());

    else
        return  ((fitGoal == 0)?1:IsDelayCritReached())
                && ((maxGens == 0)?1:IsMaxGensCritReached())
                && ((deadlock == 0)?1:IsDeadlockCritReached());
}


double CircuitSolver::BestFitness() const
{
    return f_1(fitVect.front().first);
}

void CircuitSolver::TrackEvolution(bool track, const std::string &fileName)
{
    if(dataOut.is_open())
        dataOut.close();
    if((tracking = track))
    {
        if(fileName != "")
            dataOut.open(fileName, std::ofstream::out | std::ofstream::app |std::ios_base::binary);
    }
}

