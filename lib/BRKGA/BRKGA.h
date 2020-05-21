#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>


template<class Input, class Output, class FType>
class BRKGA
{
public:
    BRKGA(size_t popSize, size_t dim, size_t pe, size_t pm, double rho_e)
        : hypercube(popSize, std::vector<double>(dim)), fitVect(popSize), pe(pe), pm(pm), rho_e(rho_e)
    {
        std::cout << "rand max: " << RAND_MAX << std::endl;
        for(size_t i = 0; i < popSize; i++)
        {
            fitVect[i].second = i;
        }
    }
    virtual ~BRKGA(){}

    Output BestSolution();
    bool Evolve();
    virtual void Decode() = 0;
    virtual bool StopCriteria() = 0;


protected:
    void Crossover(size_t p1Pos, size_t p2Pos,  std::vector<std::vector<double>>::iterator son);

    std::vector<std::vector<double>> hypercube;
    std::vector<std::pair<FType, size_t>> fitVect;

    size_t pe;
    size_t pm;

    double rho_e;
};


template<class Input, class Output, class FType>
bool BRKGA<Input, Output, FType>::Evolve()
{
    for(auto& y : hypercube)
        for(auto& x : y)
            x = static_cast<double>(rand())/RAND_MAX;


    Decode();
    std::nth_element(fitVect.begin(), fitVect.begin()+pe, fitVect.end(),
                     [](auto p1, auto p2)
    {
        return p1.first > p2.first;
    });
    size_t gens = 0;
    while(!StopCriteria())
    {
        if(gens == 32)
            std::cout << "alert" << std::endl;
        std::vector<std::vector<double>> aux(hypercube.size()-pe-pm,
                                             std::vector<double>(hypercube.front().size()) );
        for(size_t i = 0; i < aux.size(); ++i)
        {
            //takes randomly two parents
            auto p1 = hypercube[fitVect[rand()%pe].second];
            auto p2 = hypercube[fitVect[rand()%(fitVect.size()-pe)+pe].second];

            // crossover
            for(size_t j = 0 ; j < aux[i].size(); ++j)
            {
                double n = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
                aux[i][j] = (n<rho_e)?p1[j]:p2[j];
            }
        }
        std::copy(aux.begin(), aux.end(), hypercube.begin()+pe);

        for(size_t i = 0; i < pm; ++i)
        {
            size_t index = rand()%(fitVect.size()-pe) + pe;
            for(auto& x : hypercube[index])
                x = static_cast<double>(rand())/RAND_MAX;

        }
        Decode();


        std::partial_sort(fitVect.begin(), fitVect.begin()+pe, fitVect.end(),
                         [](auto p1, auto p2)
        {
            return p1.first > p2.first;
        });

        gens++;
    }
}



