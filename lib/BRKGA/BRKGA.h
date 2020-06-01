#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cmath>




struct BRKGAParams
{
    BRKGAParams(size_t popSize, size_t nAlleles, size_t pe, size_t pm, double rho_e, bool reducing = true)
        : popSize(popSize), nAlleles(nAlleles), pe(pe), pm(pm), rho_e(rho_e), reducing(reducing){}
    size_t popSize, nAlleles, pe, pm;
    double rho_e;
    bool reducing;
};



template<class Output, class FType>
class BRKGA
{
public:
    BRKGA(){};
    BRKGA(BRKGAParams p)
        : BRKGA(p.popSize, p.nAlleles, p.pe, p.pm, p.rho_e, p.reducing){}

    BRKGA(size_t popSize, size_t nAlleles, size_t pe, size_t pm, double rho_e, bool reducing = true)
        : hypercube(popSize, std::vector<double>(nAlleles)), fitVect(popSize), pe(pe), pm(pm),
          pne(popSize-pe -pm), n(nAlleles), rho_e(rho_e)
    {
        //std::cout << "rand max: " << RAND_MAX << std::endl;
        for(size_t i = 0; i < popSize; i++)
        {
            fitVect[i].second = i;
        }
        minSize = std::floor(0.2*PopSize());
        kPne = std::floor(0.1*pne);
        kPe = std::floor(0.1*pe);
        kPm = std::floor(0.1*pm);
        this->reducing = reducing;
    }
    virtual ~BRKGA(){}

    //Output BestSolution();
    bool Evolve();
    virtual void Decode() = 0;
    virtual bool StopCriteria() = 0;
    virtual double Percentile() = 0;

    void SetAutoResizing() {reducing = true;}

    virtual FType BestSolution() const {return fitVect.front().first;}

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
    size_t n;
    double rho_e;
    bool reducing;

    size_t generations;

    //reducing parameters
    double minSize;
    double kPne;
    double kPe;
    double kPm;




    double lastPercentile;
    //Output& out;
};


template<class Output, class FType>
bool BRKGA<Output, FType>::Evolve()
{
//    std::cout << "\tEvolvePopulation" << std::endl;

    InitializePopulation();
//    std::cout << "population initialized" << std::endl;

    Decode();
//    std::cout << "\tpopulation decoded" << std::endl;

    Classify();
//    for(auto& x : fitVect)
//    {
//        std::vector<double> alleles = hypercube[x.second];

//        for(auto& i : alleles)
//            std::cout << i << " ";
//        std::cout << " fit : " << x.first;
//        std::cout<< std::endl;
//    }


    lastPercentile = 0;

    generations = 0;
    std::vector<std::vector<double>> aux( pne, std::vector<double>(n) );
    while(!StopCriteria())
    {
        double percentile = Percentile();
        if(reducing && percentile > 0.5 && percentile > lastPercentile)
        {
//            std::cout << "resizing...\t " << std::endl;
            lastPercentile = percentile;
            pne -= kPne;
            pe  -= kPe;
            pm  -= kPm;
//            std::cout << "pne: " << pne
//                      << " pe: " << pe
//                      << " pm  " << pm
//                      << " min: "<< minSize
//                      << std::endl;
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
//            std::cout << "resized into: " << pne + pe + pm << " items " << std::endl;
        }

//        for(auto& x : fitVect)
//        {
//            std::vector<double> alleles = hypercube[x.second];

//            for(auto& i : alleles)
//                std::cout << i << " ";
//            std::cout << " fit : " << x.first;
//            std::cout<< std::endl;

//        }

        for(size_t i = 0; i < aux.size(); ++i)
        {
            //takes randomly two parents: form elite and non elite
            size_t eIndex = rand()%pe;
            size_t neIndex = rand()%(fitVect.size()-pe)+pe;

            // takes the correspondent index inside the hypercube
            auto parent1 = hypercube[fitVect[eIndex].second];
            auto parent2 = hypercube[fitVect[neIndex].second];

            // crossover
            for(size_t j = 0 ; j < n; ++j)
            {
                double p = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
                aux[i][j] = (p < rho_e)?parent1[j]:parent2[j];
            }
        }
        for(size_t i = 0; i < aux.size(); i++)
        {
            size_t item = fitVect[i+pe].second;
            hypercube[item] = aux[i];
        }

        for(size_t i = 0; i < pm; ++i)
        {
            size_t mutantIndex = fitVect[fitVect.size() - pm + i].second;

            for(auto& x : hypercube[mutantIndex])
                x = static_cast<double>(rand())/RAND_MAX;

        }

//        std::cout<<std::endl;
//        for(auto& x : fitVect)
//        {
//            std::vector<double> alleles = hypercube[x.second];

//            for(auto& i : alleles)
//                std::cout << i << " ";
//            std::cout << " fit : " << x.first;
//            std::cout<< std::endl;
//        }
//        std::cout<<std::endl;

        Decode();
        Classify();
//        for(auto& x : fitVect)
//        {
//            std::vector<double> alleles = hypercube[x.second];

//            for(auto& i : alleles)
//                std::cout << i << " ";
//            std::cout << " fit : " << x.first;
//            std::cout<< std::endl;
//        }
//        std::cout<<std::endl;
        generations++;
        std::cout << "generation: " << generations << std::endl;
    }
    std::cout << "end" << std::endl;
    return 0;
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



