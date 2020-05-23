#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cmath>

template<class Output, class FType>
class BRKGA
{
public:
    BRKGA(size_t popSize, size_t dim, size_t pe, size_t pm, double rho_e, bool reducing = true)
        : hypercube(popSize, std::vector<double>(dim)), fitVect(popSize), pe(pe), pm(pm), pne(popSize-pe -pm), n(dim), rho_e(rho_e)
    {
        //std::cout << "rand max: " << RAND_MAX << std::endl;
        for(size_t i = 0; i < popSize; i++)
        {
            fitVect[i].second = i;
        }
        minSize = std::floor(0.2*PopSize());
        kPne = std::floor(0.13*pne);
        kPe = std::floor(0.13*pe);
        kPm = std::floor(0.1*pm);
        this->reducing = reducing;
    }
    virtual ~BRKGA(){}

    Output BestSolution();
    bool Evolve();
    virtual void Decode() = 0;
    virtual bool StopCriteria() = 0;
    virtual double Percentile() = 0;

    void SetAutoResizing() {reducing = true;}

    size_t BestFit() const {return fitVect.front().first;}

    size_t PopSize() const {return pe + pm + pne;}

protected:
    void Crossover(size_t p1Pos, size_t p2Pos,  std::vector<std::vector<double>>::iterator son);
    void InitializePopulation();

    void Classify();

    std::vector<std::vector<double>> hypercube;
    std::vector<std::pair<FType, size_t>> fitVect;

    size_t pe;
    size_t pm;
    size_t pne;
    size_t generations;
    size_t n;
    double rho_e;

    //reducing parameters
    double minSize;
    double kPne;
    double kPe;
    double kPm;

    bool reducing;



    double lastPercentile;
    //Output& out;
};


template<class Output, class FType>
bool BRKGA<Output, FType>::Evolve()
{
    InitializePopulation();

    Decode();
    Classify();
    lastPercentile = 0;

    generations = 0;
    //reducing = true;
    std::vector<std::vector<double>> aux( pne, std::vector<double>(n) );
    while(!StopCriteria())
    {
        double percentile = Percentile();
        if(reducing && percentile > 0.5 && percentile > lastPercentile)
        {
            lastPercentile = percentile;
            pne -= kPne;
            pe  -= kPe;
            pm  -= kPm;
            if(pne < minSize)
            {
                pne = minSize-pm-pe;
                reducing = false;
                fitVect.resize(minSize);
                //fitVect.erase(fitVect.end()-minSize,  fitVect.end());
            }
            else
            {
                fitVect.erase(fitVect.end()-kPe-kPne-kPm, fitVect.end());
            }
            aux.resize(pne);
        }



        for(size_t i = 0; i < aux.size(); ++i)
        {
            //takes randomly two parents
            size_t pAI = rand()%pe;
            size_t pBI = rand()%(fitVect.size()-pe)+pe; // one from non elite population

            // takes the correspondent index inside the hypercube
            auto parent1 = hypercube[fitVect[pAI].second];
            auto parent2 = hypercube[fitVect[pBI].second];

            // crossover
            for(size_t j = 0 ; j < n; ++j)
            {
                double p = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
                aux[i][j] = (p < rho_e)?parent1[j]:parent2[j];
            }
        }
        for(size_t i = 0; i < aux.size(); i++)
        {
            hypercube[fitVect[i+pe].second] = aux[i];
        }
        for(size_t i = 0; i < pm; ++i)
        {
            size_t mutantIndex = fitVect[fitVect.size() - pm + i].second;

            for(auto& x : hypercube[mutantIndex])
                x = static_cast<double>(rand())/RAND_MAX;

        }

        Decode();
        Classify();
        generations++;
        //std::cout << BestFit() << std::endl;
    }
}

template<class Output, class FType>
void BRKGA<Output, FType>::InitializePopulation()
{
    for(auto& y : hypercube)
        for(auto& x : y)
            x = static_cast<double>(rand())/RAND_MAX;

}

// O(popSize + pe*log(pe))
template<class Output, class FType>
void BRKGA<Output, FType>::Classify()
{
    std::partial_sort(fitVect.begin(), fitVect.begin()+pe, fitVect.end(),
                      [](auto p1, auto p2)
    {
        return p1.first > p2.first;
    });
}



