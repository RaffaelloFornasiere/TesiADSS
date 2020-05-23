#include <iostream>
#include <vector>
#include "../../../lib/BRKGA/BRKGA.h"
#include <cmath>
#include <ctime>
#include <thread>
#include <mutex>
std::mutex g_display_mutex;

class StringBRKGA : public BRKGA<std::string, size_t>
{
public:
    StringBRKGA(std::string toFind, size_t pop, size_t dim, size_t pe, size_t pm, double rho_e, bool reducing = true)
        : BRKGA<std::string, size_t>(pop, dim, pe, pm, rho_e, reducing), toFind(toFind)
    {
        exp = 4;
        totFitness = pow(toFind.size(), exp);
        last = 0;
    }

    size_t Generations() { return generations;}

private:
    size_t last;
    void Decode() override;

    bool StopCriteria() override {/*Check();*/ return fitVect.front().first == totFitness;};
    double Percentile() override
    {
        double ratio = static_cast<double>(fitVect.front().first)/totFitness;
        double res = std::floor(ratio*100.0);
        return res;
    }

    void Check()
    {
        if(fitVect.front().first < last)
            std::cout<< "decrescent - "<< this->generations << std::endl;
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
    //std::cout << this->fitVect.front().first << std::endl;
}


void BRKGAExecutor(std::string toFind, size_t pop, size_t pe, size_t pm, double rho_e, bool reducing = 0)
{
    time_t start = clock();
    StringBRKGA test(toFind, pop, toFind.size(), pe, pm, rho_e, reducing);
    test.Evolve();
    start = clock()-start;

    g_display_mutex.lock();
    std::cout << "time required by thread with popsize: " << pop << " : "
              << start << std::endl
              << "generations: " << test.Generations() << std::endl
              << "final pop size: " << test.PopSize() << std::endl;
    g_display_mutex.unlock();
}

int main()
{
    std::string stest = "std::vectsddsorasdAS<double> toDecode = hypercube[i.second];";
    size_t pop = stest.size()*100;
    size_t pe = 0.15*pop, pm = 0.16*pop;
    BRKGAExecutor(stest, pop, pe, pm, 0.7);

    size_t n_threads = 6;
    std::vector<std::thread> threads;

    std::vector<size_t> pops    {100*stest.size(), 100*stest.size(), 130*stest.size(), 140*stest.size(), 150*stest.size(), 160*stest.size()};
    std::vector<double> kPe     {0.15,0.15,0.15,0.15,0.15, 0.15};
    std::vector<double> kPm     {0.2,0.2,0.2,0.2,0.16, 0.16};
    std::vector<double> kRho_e  {0.8,0.7,0.7,0.7,0.7, 0.7};
    std::vector<bool>   reducing{1, 0, 0, 0, 0, 0};

    for(size_t i = 0; i < n_threads; ++i)
        threads.emplace_back(BRKGAExecutor, stest, pops[i], kPe[i]*pops[i], kPm[i]*pops[i], kRho_e[i], reducing[i]);


    for(auto& i : threads)
        i.join();


    return 0;
}
