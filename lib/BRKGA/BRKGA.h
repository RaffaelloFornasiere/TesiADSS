#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cmath>




struct BRKGAParams
{
private:
    friend std::ostream& operator<<(std::ostream &os, const BRKGAParams &bp)
    {
        os << "init_pop_size: " << bp.popSize
           << " - init_pop_elite: " << bp.pe
           << " - init_pop_mut: " << bp.pm
           << " - init_rho_e: " << bp.rho_e;
           //<< " - reducing : " << std::boolalpha << bp.reducing;
        return os;
    }

public:
    BRKGAParams(size_t popSize, size_t nAlleles, size_t pe, size_t pm, double rho_e)//, bool reducing = true)
        : popSize(popSize), nAlleles(nAlleles), pe(pe), pne(popSize - pe -pm), pm(pm), rho_e(rho_e) //, reducing(reducing)
    {}

    size_t popSize, nAlleles, pe, pne, pm;//, minSize;
    double rho_e;
    //bool reducing;
    double ppe;
    double ppm;
};




typedef std::vector<double> DNA;


template <class FType>
class BRKGAState
{
public:
    virtual BRKGAState& operator=(std::pair<const DNA&, const FType> state)
    {dna = state.first; fitness = state.second;}


public:
    BRKGAState(DNA dna, FType fitness):dna(dna), fitness(fitness){};
    BRKGAState(){};
    bool IsTraking(){return traking;};

protected:
    DNA dna;
    bool traking;
    FType fitness;
};



template<class Input, class Output, class FType>
class BRKGA
{
public:
    BRKGA(const Input& input):input(input){};
    BRKGA(const Input& input, BRKGAParams p);
    BRKGA(const Input& input, size_t popSize, size_t nAlleles, size_t pe, size_t pm, double rho_e);
    virtual ~BRKGA(){}

    //Output BestSolution();
    bool Evolve();
    virtual void Decode() = 0;
    virtual bool StopCriteria() = 0;
    virtual double Percentile() = 0;
    virtual FType BestFitness() const {return fitVect.front().first;}

    size_t PopSize() const {return p.popSize;}

    void SimpleTest();
    void SetParams(BRKGAParams p);
    void Reset();


protected:
    void InitializePopulation();
    void Classify();

    std::vector<DNA> hypercube;
    std::vector<std::pair<FType, size_t>> fitVect;

    BRKGAParams p;

    size_t generations;

    size_t counter;
    double lastBest;


    const Input& input;
    //BRKGAState<Input, FType> current;
    //BRKGAState<FType> out, current;
};




template<class Input, class Output, class FType>
BRKGA<Input, Output, FType>::BRKGA(const Input &input, BRKGAParams p)
    : input(input), hypercube(p.popSize, DNA(p.nAlleles)),
      fitVect(p.popSize), p(p.popSize, p.nAlleles, p.pe, p.pm, p.rho_e)
{
    //std::cout << "rand max: " << RAND_MAX << std::endl;
    for(size_t i = 0; i < p.popSize; i++)
    {
        fitVect[i].second = i;
    }
    lastBest = 0;
    counter = 0;
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
    //    std::cout << "population initialized" << std::endl;

    Decode();
    //    std::cout << "\tpopulation decoded" << std::endl;

    Classify();


    generations = 0;
    std::vector<DNA> aux(p.pne, DNA(p.nAlleles));
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
        //current = std::make_pair(hypercube[fitVect.front().second], fitVect.front().second);
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
}

template<class Input, class Output, class FType>
void BRKGA<Input, Output, FType>::SetParams(BRKGAParams p)
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
    fitVect.clear();
    hypercube.clear();
    counter = 0;
    lastBest = 0;
}

template<class Input, class Output, class FType>
void BRKGA<Input, Output, FType>::InitializePopulation()
{
    for(auto& y : hypercube)
        for(auto& x : y)
            x = static_cast<double>(rand())/RAND_MAX;

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




/* double percentile = Percentile();
 if(reducing && percentile > 0.5 && percentile > lastPercentile)
 {
//            std::cout << "resizing...\t " << std::endl;
     double pReducing = (percentile-lastPercentile)/100.0;
     lastPercentile = percentile;
     popSize = static_cast<size_t>((1-pReducing)*popSize);
     pe = ppe*popSize;
     pm = ppm*popSize;
     if(pne < popSize - pe - pm)
         pe++;
     pne = popSize - pe - pm;

     if(pne < minSize)
     {
         pne = minSize-pm-pe;
         reducing = false;
         fitVect.resize(minSize);
         //fitVect.erase(fitVect.end()-minSize,  fitVect.end());
     }
     else
     {
         size_t rubbish = fitVect.size() - popSize;
         fitVect.erase(fitVect.end() - rubbish, fitVect.end());
     }
     aux.resize(pne, std::vector<double>(nAlleles));
     std::cout << " pop size: " << popSize
               << " pe: " << pe
               << " pm: " << pm
               << " pne: " << pne << " ";
 }*/


