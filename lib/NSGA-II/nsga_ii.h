#ifndef NSGA_II_H
#define NSGA_II_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>


typedef std::vector<double> DNA;


class Individual
{
public:
    Individual(const DNA& dna, size_t nObj) : dna(dna), objectives(nObj) {}

    bool Dominates(const Individual& d)
    {
        bool res = true;
        for(size_t i = 0; i < objectives.size(); i++)
        {
            if(objectives[i] > d.objectives[i])
            {
                res = 0;
                break;
            }
        }
        return res;
    }

    DNA dna;
    std::vector<double> objectives;
    size_t rank;
    std::vector<Individual*> dominated;
    size_t dominants;
    double crowdDistance;
};

struct NSGAParams
{
    NSGAParams(size_t popSize, size_t nAlleles, size_t nObj, double mutationP, size_t maxGenerations):
        popSize(popSize), nAlleles(nAlleles), nObj(nObj), mutationP(mutationP), maxGenerations(maxGenerations)
    {}
    size_t popSize;
    size_t nAlleles;
    size_t nObj;
    double mutationP;
    size_t maxGenerations;
};


template<class Input, class Output>
class NSGA_II
{
public:
    NSGA_II(const Input& input, size_t popSize, size_t nAlleles,
            size_t nObj, double mutationP = 0.01, size_t maxGenerations = 100);
    NSGA_II(const Input& input, NSGAParams p);
    virtual ~NSGA_II(){}

    bool Evolve();
    void Reset();

protected:
public:
    virtual void _Reset() = 0;
    virtual void Decode(std::vector<Individual>::iterator begin,
                        std::vector<Individual>::iterator end) = 0;
    virtual bool StopCriteria() = 0;


    void NonDominatedSort();
    void InitializePopulation();
    void CrowdingDistanceAssignment();
    bool CrowdedCompOperator(Individual& i1, Individual& i2);
    double calCrowdingDistance(Individual& i1, Individual& i2);



    const Input& input;
    Output output;

    NSGAParams p;
    std::vector<Individual> ind;

    std::vector<std::vector<Individual*>> ranks;

    size_t generations;
};


template<class Input, class Output>
NSGA_II<Input, Output>::NSGA_II(const Input &input, size_t popSize, size_t nAlleles, size_t nObj, double mutationP, size_t maxGenerations)
    : NSGA_II(input, NSGAParams(popSize, nAlleles, nObj, mutationP, maxGenerations))
{}

template<class Input, class Output>
NSGA_II<Input, Output>::NSGA_II(const Input &input, NSGAParams p)
    : input(input), output(input), p(p)
{
    ind.reserve(2*p.popSize);
    for(size_t i = 0; i < 2*p.popSize; i++)
    {
        ind.emplace_back(std::vector<double>(p.nAlleles), p.nObj);
    }
}


template<class Input, class Output>
bool NSGA_II<Input, Output>::Evolve()
{
    InitializePopulation();
    //decode all elements

    Decode(ind.begin(), ind.end());

    // sort the first N elements
    NonDominatedSort();
    CrowdingDistanceAssignment();
    std::partial_sort(ind.begin(), ind.begin() + p.popSize+1, ind.end(),
                      [](Individual& i1, Individual& i2)
    {
        return i1.rank < i2.rank;
    });
    //
    //StopCriteria();
    generations = 0;
    do
    {
        generations++;
        // reproduction
        // the upper half of the hyper cube generates the bottom half of it
        for(size_t i = p.popSize; i < ind.size(); i++)
        {
            // select 2 parents (in the first part of the hypercube)
            size_t ipA = rand()%p.popSize;
            size_t ipB = rand()%p.popSize;
            Individual& pA = (CrowdedCompOperator(ind[ipA], ind[ipB])?ind[ipA]:ind[ipB]);
            ipA = rand()%p.popSize;
            ipB = rand()%p.popSize;
            Individual& pB = (CrowdedCompOperator(ind[ipA], ind[ipB])?ind[ipA]:ind[ipB]);


            for(size_t j = 0; j < pA.dna.size(); j++)
            {
                if(static_cast<double>(rand())/RAND_MAX > p.mutationP)
                    ind[i].dna[j] = (static_cast<double>(rand())/RAND_MAX > 0.5)?pA.dna[j]:pB.dna[j];
                else
                    ind[i].dna[j] = static_cast<double>(rand())/RAND_MAX;
            }
        }

        //decode only the bottom half of the hypercube, the upper is already decoded
        Decode(ind.begin(), ind.end());
        NonDominatedSort();
        CrowdingDistanceAssignment();
        std::partial_sort(ind.begin(), ind.begin() + p.popSize+1, ind.end(),
                          [](Individual& i1, Individual& i2)
        {
            return i1.rank < i2.rank;
        });


        size_t elemCount = 0;
        size_t i = 0;
        while(elemCount < p.popSize)
            elemCount += ranks[i++].size();

        i--;
        elemCount -= ranks[i].size();

        std::partial_sort(ind.begin() + elemCount, ind.begin() + p.popSize+1, ind.end(),
                          [](Individual& i1, Individual& i2)
        {
            return i1.crowdDistance > i2.crowdDistance;
        });


    }while(StopCriteria());
    return 1;
}

template<class Input, class Output>
void NSGA_II<Input, Output>::Reset()
{
    _Reset();
}

template<class Input, class Output>
void NSGA_II<Input, Output>::NonDominatedSort()
{
    //clean-up
    ranks.clear();
    std::for_each(ind.begin(), ind.end(), [](Individual& i)
    {
        i.dominants = 0;
        i.rank = -1;
        i.dominated.clear();
    });

    //sorting
    std::vector<Individual*> front;
    for(Individual& x : ind)
    {
        for(Individual& y : ind)
        {
            if (x.Dominates(y))
                x.dominated.push_back(&y);
            else if (y.Dominates(x))
                x.dominants++;
        }
        if(x.dominants == 0)
        {
            x.rank = 1;
            front.push_back(&x);
            std::sort(front.begin(), front.end(), [](Individual* i1, Individual* i2)
            {
                return i1->objectives[0] > i2->objectives[0];
            });
        }
    }
    ranks.push_back(front);

    /*******************************************************************/
    /*size_t     j = 0;
    for(auto& v : ranks)
    {
        //std::cout << "front: " << j++ << std::endl;
        std::sort(v.begin(), v.end(), [](Individual* i1, Individual* i2)
        {
            return i1->objectives[0] > i2->objectives[0];
        });
        bool res = std::is_sorted(v.begin(), v.end(), [](Individual* i1, Individual* i2)
        {
                return i1->objectives[1] > i2->objectives[1];
        });
        if(res)
            throw std::logic_error("aaa2");

    }*/
    /*******************************************************************/


    size_t rankCount = 2;
    size_t elemCount = 0;
    while(front.size() != 0)
    {
        std::vector<Individual*> nextFront;
        for(Individual* i : front)
        {
            for(Individual* j : i->dominated)
            {
                j->dominants--;
                if(j->dominants == 0)
                {
                    nextFront.push_back(j);
                    j->rank = rankCount;
                }
            }

        }

        // need to sort only the first ceil(N) elements
        elemCount += front.size();
        if(elemCount > p.popSize)
            break;

        front = nextFront;
        ranks.push_back(front);
        rankCount++;
    }


    /*******************************************************************/
    /*j = 0;
    for(auto& v : ranks)
    {
        //std::cout << "front: " << j++ << std::endl;
        std::sort(v.begin(), v.end(), [](Individual* i1, Individual* i2)
        {
            return i1->objectives[0] > i2->objectives[0];
        });
        bool res = std::is_sorted(v.begin(), v.end(), [](Individual* i1, Individual* i2)
        {
                return i1->objectives[1] > i2->objectives[1];
        });
        if(res)
            throw std::logic_error("aaa2");

    }*/
    /*******************************************************************/


}

template<class Input, class Output>
void NSGA_II<Input, Output>::InitializePopulation()
{
    for(auto& i : ind)
        for(auto& j : i.dna)
            j = static_cast<double>(rand())/RAND_MAX;
}

template<class Input, class Output>
void NSGA_II<Input, Output>::CrowdingDistanceAssignment()
{
    // calculates all upper and lower limits for each objective
    std::vector<double> den(p.nObj);
    for(size_t m = 0; m < p.nObj; m++)
    {
        std::pair<double, double> boundaries(-1, std::numeric_limits<double>::infinity());
        for(Individual& i : ind)
        {
            if(i.objectives[m] > boundaries.first)
                boundaries.first = i.objectives[m];
            if(i.objectives[m] < boundaries.second && boundaries.second != -1)
                boundaries.second = i.objectives[m];
        }
        den[m] = std::abs(boundaries.first-boundaries.second);
    }


    size_t count = 0;
    for(std::vector<Individual*> v : ranks)
    {
        for(Individual* i : v)
            i->crowdDistance = 0;

        for(size_t m = 0; m < p.nObj; m++)
        {
            std::sort(v.begin(), v.end(), [=](Individual* i1, Individual* i2)
            {
                return i1->objectives[m] < i2->objectives[m];
            });

            v.front()->crowdDistance = std::numeric_limits<double>::infinity();
            v.back()->crowdDistance = std::numeric_limits<double>::infinity();
            for(size_t i = 1; i < v.size()-1; i++)
            {
                v[i]->crowdDistance += std::abs(v[i+1]->objectives[m] - v[i-1]->objectives[m])/den[m];
            }

        }

        // need to calculate only ceil(N) elements.
        count += v.size();
        if(count > p.popSize)
            break;
    }

}

template<class Input, class Output>
bool NSGA_II<Input, Output>::CrowdedCompOperator(Individual &i1, Individual &i2)
{
    if(i1.rank < i2.rank)
        return 1;
    else if(i1.rank > i2.rank)
        return 0;
    else if(i1.crowdDistance > i2.crowdDistance)
        return 1;
    else
        return 0;
}

#endif // NSGA_II_H
