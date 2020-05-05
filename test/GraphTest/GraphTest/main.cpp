#include <iostream>
#include <../../../lib/Graph/graph.h>
#include <list>
#include <algorithm>
#include <ctime>
#include <fstream>


int main()
{
    size_t n = 10;
    Graph<int, double> a;
    //srand(time(nullptr));
    std::ofstream outFile("../../../files/output-adiacenza.txt", std::ios_base::out|std::ios_base::binary );


    //std::vector<int> items; items.reserve(20);

    std::vector<Graph<int, double>::GraphNode*> items; items.reserve(n);
    for(size_t i = 0; i < n; i++)
    {
        a.adjList.emplace_back(i);
        items.push_back(&a.adjList.back());
    }

    size_t j = 0;
    for(auto& i : a.adjList)
    {
        //std::cout << "\"" << j << "\"";
        outFile  << "\"" << j << "\"";
        for(size_t k = 0; k < n/2; k++)
        {
            if((rand()%100) > 90 - j/(n-1)*85)
                break;
            int r = 0;
            if(j <n-1)
                r = rand()%(n-j-1) + j+1;
            i.adj.push_back(items[r]);
            //std::cout << "->\"" << r <<"\"";
            outFile << "->\"" << r <<"\"";

        }
        //std::cout <<std::endl;
        outFile << std::endl;
        j++;

    }
    time_t start = clock();
    a.TopologicalSort();
    double f = a.getWorstPathDistance();
    std::cout << "worst path: " << f << " time required: " << clock() - start << std::endl;


    return 0;
}
