#include <iostream>
#include <vector>
#include "../../../lib/BRKGA/BRKGA.h"
#include <cmath>
#include <ctime>

class StringBRKGA : public BRKGA<std::string, std::string, size_t>
{
public:
    StringBRKGA(std::string toFind, size_t pop, size_t dim, size_t pe, size_t pm, double rho_e)
        : BRKGA<std::string, std::string, size_t>(pop, dim, pe, pm, rho_e), toFind(toFind)
    {
        exp = 4;
        totFitness = pow(toFind.size(), exp);
        last = 0;
    }

private:
    size_t last;
    void Decode() override;

    bool StopCriteria() override {Check(); return fitVect.front().first == totFitness;};

    void Check()
    {
        if(fitVect.front().first < last)
            std::cout<< "decrescent" << std::endl;
        last = fitVect.front().first;
    }
    size_t totFitness;
    double exp;
    std::string toFind;
};

void StringBRKGA::Decode()
{
    for(auto& i : fitVect)
    {
        std::vector<double> toDecode = hypercube[i.second];
        i.first = 0;
        int k = 0;
        for(size_t j = 0; j < toDecode.size(); j++)
        {
            char c = toDecode[j]*255;
            char t = toFind[j];
            if(c == t)
                k++;
        }
        i.first = pow(k,exp);
    }
}

int main()
{

    std::string stest = "std::vectsddsor<double> toDecode = hypercube[i.second];";
    size_t pop = stest.size()*9;
    size_t pe = 0.1*pop, pm = 0.16*pop;

    time_t start = clock();
    StringBRKGA test(stest, pop, stest.size(), pe, pm, 0.8);
    test.Evolve();
    std::cout << "time: " << clock() - start << std::endl;

    return 0;
}
