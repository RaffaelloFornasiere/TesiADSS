#include "circuitsolver.h"



CircuitSolver::CircuitSolver(const CircuitInput &input, const BRKGAParams &params)
    : BRKGA<CircuitInput, CircuitOutput, double>(input, params)
{
    deadlock = 0;
    maxGens = 0;
    fitGoal = 0;
    fitnessExp = 3;
    counter = 0;
    lastBest = 0;
    k = 100;
    lastTimeRec = clock();

    output.getWorstPathDistance();
}


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
    if(fitVect.front().first <= lastBest)
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
   // output.getWorstPathDistance();
    for(auto& item : fitVect)
    {
        //time_t start = clock();
        std::vector<double> alleles = hypercube[item.second];
        output.SetAllCells(alleles);

        output.SetupCaps();
        //double area = output.GetAreaOccupation();
        item.first = f(output.getWorstPathDistance());
    }
}


bool CircuitSolver::StopCriteria()
{
    if(tracking)
    {
        std::string s = "d: " + std::to_string(f_1(fitVect.front().first));
        output.SetAllCells(hypercube[fitVect.front().second]);
        double area = output.GetAreaOccupation();
        s += " a: " + std::to_string(area);
        if(timeTracking)
        {
            double secs = double(clock() - lastTimeRec)/CLOCKS_PER_SEC;
            s += " t: " + std::to_string(secs);
        }


        if(dataOut.is_open())
            dataOut << s << std::endl;
        else
            std::cout << s << std::endl;
    }

    if(criteriaPolicy == CriteriaPolicy::OrPolicy)
    {

        return  ((fitGoal == 0)?0:IsDelayCritReached())
                || ((maxGens == 0)?0:IsMaxGensCritReached())
                || ((deadlock == 0)?0:IsDeadlockCritReached());
    }
    else
    {
        bool b = ((fitGoal == 0)?1:IsDelayCritReached())
                && ((maxGens == 0)?1:IsMaxGensCritReached())
                && ((deadlock == 0)?1:IsDeadlockCritReached());
        return b&&((fitGoal + maxGens + deadlock) != 0);
    }
}


double CircuitSolver::BestFitness() const
{
    return f_1(fitVect.front().first);
}

void CircuitSolver::TrackEvolution(bool track, const std::string &fileName, bool timeTracking)
{
    this->timeTracking = timeTracking;
    if(dataOut.is_open())
        dataOut.close();
    if((tracking = track))
    {
        if(fileName != "")
            dataOut.open(fileName, std::ofstream::out | std::ofstream::app |std::ios_base::binary);
    }
}

