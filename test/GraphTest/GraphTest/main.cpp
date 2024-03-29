#include <iostream>
#include "../../../lib/Graph/graph.h"
#include <list>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cmath>



class MyNode : public GraphNode<double>
{
public:
    MyNode(int i) : i (i) {}
    ~MyNode() {}
    int i;
};

class MyGraph : public Graph<double>
{
public:
    MyGraph(size_t n, size_t maxEdges, std::ostream& os);
private:

};


MyGraph::MyGraph(size_t n_i, size_t maxEdges, std::ostream &os)
{
    for(size_t pw = 0; pw < 7; pw++)
    {
        size_t n = pow(n_i, pw);
        std::cout << "exec:" << std::setw(10) << n << "\tnodes" << std::endl;
        //std::vector<MyNode*> items; items.reserve(n);
        for(size_t i = 0; i < n; i++)
        {
            AddNode(new MyNode(i));
        }

        size_t j = 0;
        for(auto& i : adjList)
        {
            //std::cout << "\"" << j << "\"";
            //os  << "\"" << j << "\"";
            if(j != n-1)
            for(size_t k = 0; k < n_i; k++)
            {

                size_t r = rand()%100;
                //size_t ref = 90 - double(j)/(n-1)*70;
                if(rand()%100 > (90 - double(j)/(n-1)*(50)))
                    break;

                if(j < n-1)
                    r = rand()%(n-j-1) + j+1;

                i->AddNeighbor(getNode(r));

                //std::cout << "->\"" << r <<"\"";
                //os << "->\"" << r <<"\"";

            }
            //std::cout <<std::endl;
            //os << std::endl;
            j++;
        }

        std::cout << "topological sort:" << std::endl;

        time_t start = clock();
        TopologicalSort();

        std::cout << "\t\tsorted:" << std::setw(29) << n << " nodes in:" << std::setw(6) << clock()-start << " ms" << std::endl;

        start = clock();
        double f = getWorstPathDistance();

        std::cout << "\t\tcomputed worst path among:" << std::setw(10) << n << " nodes in:" << std::setw(6) << clock()-start << " ms" << std::endl;
        std::cout << "\t\tworst path costs:"<< std::setw(19) << f << std::endl<< std::endl;

        while(NumNodes() > 0)
        {
            DeleteNode(0);
        }
    }
}

class Item
{
public:
    Item (int item) : item(item) {std::cout << "create" << std::endl;}
    ~Item(){std::cout << "delete" << std::endl;}
private:
    int item;
};


int main()
{
    int n = 10;
    srand(time(nullptr));
    std::ofstream outFile("../../../files/output-adiacenza.txt", std::ios_base::out|std::ios_base::binary );

    MyGraph mg (n, 10, outFile);


    return 0;
}

