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
public:
    GraphNode(M item):item(item){/*std::cout << "GraphNode constructor" << std::endl;*/}
    virtual ~GraphNode(){/*std::cout << "GraphNode destructor" << std::endl;*/}
    BinHeapNode<GraphNode*, M>* heapPtr;
    M item;
    std::vector<GraphNode*> adj;
    virtual M Distance(GraphNode* a) {return adj.size();}

    friend class Graph<M>;
};


template <class M>
class Graph
{
public:


public:
    Graph(){}
    void TopologicalSort();
    M getWorstPathDistance();
    M OrderAndGetWorstDistance();


public:  
    //virtual M Distance(GraphNode<T,M> n1, GraphNode<T,M> n2);
    std::list<GraphNode<M>> adjList;
    std::vector<GraphNode<M>*> sorted;
};
#endif // GRAPH_H


template<class M>
void Graph<M>::TopologicalSort()
{
    BinHeap<GraphNode<M>*, M> heap(adjList.size(), BinHeap<GraphNode<M>*,M>::minHeap);
    //std::cout << "init heap" << std::endl;
    for(GraphNode<M>& x : adjList)
    {
        if(!heap.Contains(x.heapPtr))
            x.heapPtr = heap.Push(&x, 0);
        for(GraphNode<M>* y : x.adj)
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


