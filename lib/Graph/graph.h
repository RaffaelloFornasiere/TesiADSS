#ifndef GRAPH_H
#define GRAPH_H
#include "../BinHeap/binheap.h"
#include <vector>
#include <list>
#include <unordered_set>

template < class T, class M > class Graph;

template <class T, class M>
class GraphNode
{
    friend bool operator==(const GraphNode& a, const GraphNode& b) { return a.item == b.item; }
public:
    GraphNode(T item):item(item){/*std::cout << "GraphNode constructor" << std::endl;*/}
    virtual ~GraphNode(){/*std::cout << "GraphNode destructor" << std::endl;*/}
    BinHeapNode<GraphNode*, M>* heapPtr;
    T item;
    std::vector<GraphNode*> adj;
    virtual M Distance(T item) {return adj.size();}

    friend class Graph<T,M>;
    //virtual bool IsSource() {};
    //virtual bool IsSink() {};
};


template <class T, class M>
class Graph
{
public:


public:
    Graph(){}

    void TopologicalSort();
    M getWorstPathDistance();
    M OrderAndGetWorstDistance();
public:
    std::list<GraphNode<T,M>> adjList;
    std::vector<GraphNode<T,M>*> sorted;
};
#endif // GRAPH_H


template<class T, class M>
void Graph<T,M>::TopologicalSort()
{
    BinHeap<GraphNode<T,M>*, M> heap(adjList.size(), BinHeap<GraphNode<T,M>*,M>::minHeap);
    //std::cout << "init heap" << std::endl;
    for(GraphNode<T,M>& x : adjList)
    {
        if(!heap.Contains(x.heapPtr))
            x.heapPtr = heap.Push(&x, 0);
        for(GraphNode<T,M>* y : x.adj)
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
        std::pair<GraphNode<T,M>*, M> e = heap.Front();
        sorted.push_back(e.first);
        for(GraphNode<T,M>* x : e.first->adj)
        {
            heap.DecreaseKey(x->heapPtr, 1);
        }
        heap.Pop();
    }
    //std::cout << "heap empty" << std::endl;

    return;

}

template<class T, class M>
M Graph<T,M>::getWorstPathDistance()
{
    std::unordered_map<GraphNode<T,M>*, M> distances;

    for(GraphNode<T,M>* x : sorted)
    {
        for(GraphNode<T,M>* y : x->adj)
        {
            M dist = x->Distance(y->item);
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

