#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cmath>
#include <fstream>



struct BRKGAParams
{
private:
    friend std::ostream& operator<<(std::ostream &os, const BRKGAParams &bp)
    {
        os << "pop_size: " << bp.popSize
           << " - pop_elite: " << bp.pe
           << " - pop_mut: " << bp.pm
           << " - pop_nonelite: " << bp.pne
           << " - rho_e: " << bp.rho_e;
        return os;
    }

public:
    BRKGAParams(size_t popSize, size_t nAlleles, size_t pe, size_t pm, double rho_e)//, bool reducing = true)
        : popSize(popSize), nAlleles(nAlleles), pe(pe), pne(popSize - pe -pm), pm(pm), rho_e(rho_e) //, reducing(reducing)
    {}
    BRKGAParams(){Reset();}

    void UpdateFromRate()
    {
        pe = static_cast<size_t>(popSize*ppe);
        pm = static_cast<size_t>(popSize*ppm);
        pne = static_cast<size_t>(popSize-pm-pe);
    }

    void UpdateFromPop()
    {
        ppe = double(pe)/popSize;
        ppm = double(pm)/popSize;
        pne = popSize-pe-pm;
    }

    void Reset()
    {
        popSize = nAlleles = pe = pne = pm = 0;
        rho_e = ppe = ppm = 0;
    }

    size_t popSize, nAlleles, pe, pne, pm;
    double rho_e;
    double ppe;
    double ppm;
    uint8_t stopCriterias;
};

typedef std::vector<double> DNA;



template<class Input, class Output, class FType>
class BRKGA
{
public:
    BRKGA(const Input& input):input(input), output(input){ bias = -1;}
    BRKGA(const Input& input, const BRKGAParams &p);
    BRKGA(const Input& input, size_t popSize, size_t nAlleles, size_t pe, size_t pm, double rho_e);
    virtual ~BRKGA(){}

    // ******************** User Interaction Members *******************/
    bool Evolve();
    virtual FType BestFitness() const {return fitVect.front().first;}
    void SimpleTest();  // performs only one cycle of evolution
    void SetParams(const BRKGAParams &p);
    void Reset();
    size_t PopSize() const {return p.popSize;}
    // ******************************************************************

    double getBias() const;
    void setBias(double value);

protected:
    // *************************** Hotspots  *****************************************
    virtual void _Reset() = 0;     //called by Reset(). Reset derived class data
    virtual void Decode() = 0;
    virtual bool StopCriteria() = 0;
    // *******************************************************************************

    void InitializePopulation();
    void Classify();

    // ********** Algorithm data **************
    std::vector<DNA> hypercube;
    std::vector<std::pair<FType, size_t>> fitVect;

    BRKGAParams p;
    size_t generations;
    // ****************************************

    double bias;

    const Input& input;
    Output output;
};

template<class Input, class Output, class FType>
BRKGA<Input, Output, FType>::BRKGA(const Input &input, const BRKGAParams& p)
    : hypercube(p.popSize, DNA(p.nAlleles)), fitVect(p.popSize),
      p(p.popSize, p.nAlleles, p.pe, p.pm, p.rho_e), input(input),  output(input)
{
    for(size_t i = 0; i < p.popSize; i++)
    {
        fitVect[i].second = i;
    }
    bias = -1;
}

template<class Input, class Output, class FType>
BRKGA<Input, Output, FType>::BRKGA(const Input &input, size_t popSize, size_t nAlleles, size_t pe, size_t pm, double rho_e)
    :BRKGA(input, BRKGAParams(popSize, nAlleles, pe, pm, rho_e))
{}

template<class Input, class Output, class FType>
bool BRKGA<Input, Output, FType>::Evolve()
{
    //    std::cout << "\tEvolvePopulation" << std::endl;
    InitializePopulation();
    //    std::cout << "\tpopulation initialized" << std::endl;
    Decode();
    //    std::cout << "\tpopulation decoded" << std::endl;
    Classify();
    //    std::cout << "\tpopulation classified" << std::endl;
    generations = 0;
    std::vector<DNA> aux(p.pne, DNA(p.nAlleles));

    //std::cout << "ready to entry while" << std::endl;
    while(!StopCriteria())
    {
        for(size_t i = 0; i < p.pne; ++i)
        {
            //takes randomly two parents: form elite and non elite
            size_t eIndex = rand()%p.pe;
            size_t neIndex = rand()%(p.popSize-p.pe)+p.pe;

            // takes the correspondent index inside the hypercube
            auto parent1 = hypercube[fitVect[eIndex].second];
            auto parent2 = hypercube[fitVect[neIndex].second];

            // crossover
            for(size_t j = 0 ; j < p.nAlleles; ++j)
            {
                double prob = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
                aux[i][j] = (prob < p.rho_e)?parent1[j]:parent2[j];
            }
        }
        for(size_t i = 0; i < p.pne; i++)
        {
            size_t item = fitVect[i+p.pe].second;
            hypercube[item] = aux[i];
        }

        for(size_t i = 0; i < p.pm; ++i)
        {
            size_t mutantIndex = fitVect[p.popSize - p.pm + i].second;

            for(auto& x : hypercube[mutantIndex])
                x = static_cast<double>(rand())/RAND_MAX;
        }

        Decode();
        Classify();

        generations++;
    }
    std::cout << "end" << std::endl;
    return 0;
}

template<class Input, class Output, class FType>
void BRKGA<Input, Output, FType>::SimpleTest()
{
    InitializePopulation();
    //    std::cout << "population initialized" << std::endl;

    Decode();
    //    std::cout << "\tpopulation decoded" << std::endl;

    Classify();

    StopCriteria();
}

template<class Input, class Output, class FType>
void BRKGA<Input, Output, FType>::SetParams(const BRKGAParams& p)
{
    this->p = p;
    fitVect.resize(p.popSize);

    for(size_t i = 0; i < p.popSize; i++)
    {
        fitVect[i].second = i;
    }
    hypercube.resize(p.popSize, DNA(p.nAlleles));
}

template<class Input, class Output, class FType>
void BRKGA<Input, Output, FType>::Reset()
{
    _Reset();
    fitVect.clear();
    hypercube.clear();
    generations = 0;
    p.Reset();
}

template<class Input, class Output, class FType>
void BRKGA<Input, Output, FType>::InitializePopulation()
{
    for(auto& y : hypercube)
        for(auto& x : y)
            if(bias < 0)
                x = static_cast<double>(rand())/RAND_MAX;
            else
                x = bias;

}

// O(popSize + pe*log(pe))
template<class Input, class Output, class FType>
void BRKGA<Input, Output, FType>::Classify()
{
    std::partial_sort(fitVect.begin(), fitVect.begin()+p.pe, fitVect.end(),
                      [](auto p1, auto p2)
    {
        return p1.first > p2.first;
    });
}


template<class Input, class Output, class FType>
double BRKGA<Input, Output, FType>::getBias() const
{
    return bias;
}

template<class Input, class Output, class FType>
void BRKGA<Input, Output, FType>::setBias(double value)
{
    if(value > 1.000001 || value < -0.00000001)
        bias = -1.0;
    else
        bias = value;
}
