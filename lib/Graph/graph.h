#ifndef GRAPH_H
#define GRAPH_H
#include "../BinHeap/binheap.h"
#include <vector>
#include <list>
#include <unordered_set>

template <class M > class Graph;

template <class M>
class GraphNode
{
    friend bool operator==(const GraphNode& a, const GraphNode& b) { return a.item == b.item; }
    friend class Graph<M>;

public:
    GraphNode(){/*std::cout << "GraphNode constructor" << std::endl;*/}
    virtual ~GraphNode(){/*std::cout << "GraphNode destructor" << std::endl;*/}

    virtual M Distance(GraphNode* a) {return adj.size();}
    void AddNeighbor(GraphNode* a){adj.push_back(a);}

private:
    BinHeapNode<GraphNode*, M>* heapPtr;
    //M item;
    std::vector<GraphNode*> adj;
};


template <class M>
class Graph
{

public:
    Graph(){}
    void TopologicalSort();
    M getWorstPathDistance();
    M OrderAndGetWorstDistance();

    virtual void AddNode(GraphNode<M>*a) {adjList.push_back(a);}
    virtual void DeleteNode(int i);

    GraphNode<M>* getNode(int node);

public:  
    //virtual M Distance(GraphNode<T,M> n1, GraphNode<T,M> n2);
    std::list<GraphNode<M>*> adjList;
    std::vector<GraphNode<M>*> sorted;
};
#endif // GRAPH_H


template<class M>
void Graph<M>::TopologicalSort()
{
    BinHeap<GraphNode<M>*, M> heap(adjList.size(), BinHeap<GraphNode<M>*,M>::minHeap);
    //std::cout << "init heap" << std::endl;
    for(GraphNode<M>* x : adjList)
    {
        if(!heap.Contains(x->heapPtr))
            x->heapPtr = heap.Push(x, 0);
        for(GraphNode<M>* y : x->adj)
        {
            if(heap.Contains(y->heapPtr))
                heap.IncreaseKey(y->heapPtr, 1);
            else
                y->heapPtr = heap.Push(y, 1);
        }
    }
    //std::cout << "heap created" << std::endl;

    if(heap.Front().second != 0)
        throw std::logic_error("the graph is not a DAG");

    while(!heap.Empty())
    {
        std::pair<GraphNode<M>*, M> e = heap.Front();
        sorted.push_back(e.first);
        for(GraphNode<M>* x : e.first->adj)
        {
            heap.DecreaseKey(x->heapPtr, 1);
        }
        heap.Pop();
    }
    //std::cout << "heap empty" << std::endl;

    return;

}

template< class M>
M Graph<M>::getWorstPathDistance()
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
        {
            max = i.second;
        }
    }

    return max;
}

template<class M>
void Graph<M>::DeleteNode(int node)
{
    auto it = adjList.begin();
    for(int i = 0; i < node; i++)
    {
        it++;
    }
    delete *it;
    adjList.erase(it);
}

template<class M>
GraphNode<M> *Graph<M>::getNode(int node)
{
    auto it = adjList.begin();
    for(int i = 0; i < node; i++)
    {
        it++;
    }
    return *it;
}


