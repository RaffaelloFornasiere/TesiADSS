#include <iostream>
#include "../../../lib/Graph/graph.h"
#include <list>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cmath>

int main()
{
    size_t n = 10;

    srand(time(nullptr));
    std::ofstream outFile("../../../files/output-adiacenza.txt", std::ios_base::out|std::ios_base::binary );

    //bool a = 1;
    for(int times = 1; times < 4; times++)
    {
        n = pow(80, times);
        std::cout << "exec:" << std::setw(10) << n << "\tnodes" << std::endl;
        Graph<int, double> a;
        std::vector<GraphNode<int, double>*> items; items.reserve(n);
        for(size_t i = 0; i < n; i++)
        {
            a.adjList.emplace_back(i);
            items.push_back(&a.adjList.back());
        }

        //std::cout << "init nodes" << std::endl;

        size_t j = 0;
        for(auto& i : a.adjList)
        {
            //std::cout << "\"" << j << "\"";
            //outFile  << "\"" << j << "\"";
            for(size_t k = 0; k < n/2; k++)
            {
                if((rand()%100) > 90 - j/(n-1)*85)
                    break;
                int r = 0;
                if(j <n-1)
                    r = rand()%(n-j-1) + j+1;
                i.adj.push_back(items[r]);
                //std::cout << "->\"" << r <<"\"";
                //outFile << "->\"" << r <<"\"";

            }
            //std::cout <<std::endl;
            //outFile << std::endl;
            j++;

        }
        std::cout << "topological sort:" << std::endl;

        time_t start = clock();
        a.TopologicalSort();

        std::cout << "\t\tsorted:" << std::setw(29) << n << " nodes in:" << std::setw(6) << clock()-start << " ms" << std::endl;

        start = clock();
        double f = a.getWorstPathDistance();

        std::cout << "\t\tcomputed worst path among:" << std::setw(10) << n << " nodes in:" << std::setw(6) << clock()-start << " ms" << std::endl;
        std::cout << "\t\tworst path costs:"<< std::setw(19) << f << std::endl<< std::endl;
    }

    return 0;
}
