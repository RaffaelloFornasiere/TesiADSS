#ifndef GRAPH_H
#define GRAPH_H
#include "../BinHeap/binheap.h"
#include <vector>
#include <list>
#include <unordered_set>
#include <iostream>

template <class M > class Graph;

//  ***********************************************************************************************************
//  A graph data structure for DAGs implemented as an interface. It uses a binary heap to create a sorted
//  vector that is used to calculate the worst path.
//  The two main methods of the class are the one that performs the topological sort and the one that calculate
//  the distance. The second one is a DAG-shortes-path that works with multisources DAGs.
//  If n is the number of the nodes, the time cost of the topological sort is O(nlog(n)) and the one of
//  the distance algorihtm is O(n*d) where d is the average density of the graph.
//  The interface is not fully featured yet.
//  ***********************************************************************************************************


template <class M>
class GraphNode
{
    friend bool operator==(const GraphNode& a, const GraphNode& b) { return a.item == b.item; }
    template<class N>
    friend std::ostream& operator<<(std::ostream& os, const GraphNode<N>& gn);

    friend class Graph<M>;

public:
    GraphNode(const std::string& name = ""):name(name){}
    virtual ~GraphNode(){}
    virtual M Distance(GraphNode* a) = 0;
    std::string getName() const {return name;}
    virtual void AddNeighbor(GraphNode* a){adj.push_back(a);}

protected:
    std::string name;
    std::vector<GraphNode*> adj;

private:
    BinHeapNode<GraphNode*, M>* heapPtr;
};

template <class M>
class Graph
{
    template<class N>
    friend std::ostream& operator<<(std::ostream& os, const Graph<N>& g);

public:
    Graph(){}
    virtual ~Graph(){}

    // **********************************************************************
    void TopologicalSort();
    M getWorstPathDistance() const;

    virtual void DeleteNode(int i);

    GraphNode<M>* getNode(int node);
    // **********************************************************************

protected:
    std::vector<GraphNode<M>*> adjList;
    std::vector<GraphNode<M>*> sorted;
};



template<class M>
std::ostream& operator<<(std::ostream &os, const Graph<M> &g)
{
    for(size_t i = 0; i < g.adjList.size(); i++)
    {
        os << *g.adjList[i] << '\n';
    }
    return os;
}


//  ********************************************************************************************
//  In the first part it constructs the heap in order to keep in the top the elements that have less
//  input connections. A the end of the heap the first node should be a source. Otherwise there is a cycle
//

template<class M>
void Graph<M>::TopologicalSort()
{
    //std::cout << "\t\t adjsize: "<<adjList.size() <<std::endl;

    BinHeap<GraphNode<M>*, M> *heap = new BinHeap<GraphNode<M>*, M>(adjList.size(), BinHeap<GraphNode<M>*, M>::minHeap);
    for(GraphNode<M>* x : adjList)
    {     
        if(!heap->Contains(x->heapPtr))
        {
            x->heapPtr = heap->Push(x, 0);
        }


        for(GraphNode<M>* y : x->adj)
        {
          //  std::cout << "\t" << y->name;
            if(heap->Contains(y->heapPtr))
            {
               // std::cout << "c" << std::endl;
                heap->IncreaseKey(y->heapPtr, 1);
            }
            else
            {
              //  std::cout << "nc" << std::endl;
                y->heapPtr = heap->Push(y, 1);
            }
        }
    }


    while(!heap->Empty())
    {
        //std::cout << heap->Front().first->name << " "<< heap->Front().second << std::endl;
//        keys2.push_back(heap->Front().second);
        if(heap->Front().second != 0.0)
        {
            heap->Front();
            //throw std::logic_error("the graph is not a DAG");
        }
        std::pair<GraphNode<M>*, M> e = heap->Front();
        sorted.push_back(e.first);
        for(GraphNode<M>* x : e.first->adj)
        {
            heap->DecreaseKey(x->heapPtr, 1);
        }

        heap->Pop();

    }

    delete heap;
    return;
}

template< class M>
M Graph<M>::getWorstPathDistance() const
{
    std::unordered_map<GraphNode<M>*, M> distances;


    for(GraphNode<M>* x : sorted)
    {
        for(GraphNode<M>* y : x->adj)
        {
            M dist = x->Distance(y);
            if(dist + distances[x] > distances[y])
                distances[y] = dist + distances[x];
        }
    }


    M max(0);
    for(auto i : distances)
    {
        if(max < i.second)
            max = i.second;
    }
    return max;
}

template<class M>
void Graph<M>::DeleteNode(int node)
{
    auto it = adjList.begin() + node;
    sorted.erase(sorted.begin()+node);
    adjList.erase(it);
}

template<class M>
GraphNode<M> *Graph<M>::getNode(int node)
{
    auto it = adjList.begin() + node;
    return *it;
}



template<class N>
std::ostream& operator<<(std::ostream &os, const GraphNode<N> &gn)
{
    os << "\"" << gn.name << "\"";
    for(GraphNode<N>* i : gn.adj)
        os << "->\"" << i->name << "\"";
    return  os;
}
#endif // GRAPH_H


