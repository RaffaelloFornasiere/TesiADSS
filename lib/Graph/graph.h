#ifndef GRAPH_H
#define GRAPH_H
#include "../../BinaryHeap/BinaryHeap/binheap.h"
#include <vector>
#include <list>
#include <unordered_set>



template <class T, class M>
class Graph
{
public:
    class GraphNode
    {
        friend bool operator==(const GraphNode& a, const GraphNode& b) { return a.item == b.item; }
    public:
        GraphNode(T item):item(item){}
        virtual ~GraphNode(){}
        BinHeapNode<GraphNode*, M>* heapPtr;
        T item;
        std::vector<GraphNode*> adj;
        virtual M Distance(T item) {return adj.size();}

        friend class Graph;
        virtual bool IsSource() {};
        virtual bool IsSink() {};
    };

public:
    Graph(){}

    void TopologicalSort();
    M getWorstPathDistance();
    M OrderAndGetWorstDistance();
public:
    std::list<GraphNode> adjList;
    std::vector<GraphNode*> sorted;
};
#endif // GRAPH_H


template<class T, class M>
void Graph<T,M>::TopologicalSort()
{
    BinHeap<GraphNode*, M> heap(adjList.size(), BinHeap<GraphNode*,M>::minHeap);
    for(GraphNode& x : adjList)
    {
        if(!heap.Contains(x.heapPtr))
            x.heapPtr = heap.Push(&x, 0);
        for(GraphNode* y : x.adj)
        {
            if(heap.Contains(y->heapPtr))
                heap.IncreaseKey(y->heapPtr, 1);
            else
                y->heapPtr = heap.Push(y, 1);
        }
    }

    if(heap.Front().second != 0)
        throw std::logic_error("the graph is not a DAG");

    while(!heap.Empty())
    {
        std::pair<GraphNode*, M> e = heap.Pop();
        sorted.push_back(e.first);
        for(GraphNode* x : e.first->adj)
        {
            heap.DecreaseKey(x->heapPtr, 1);
        }
    }

    return;

}

template<class T, class M>
M Graph<T,M>::getWorstPathDistance()
{
    std::unordered_map<GraphNode*, M> distances;

    for(GraphNode* x : sorted)
    {
        for(GraphNode* y : x->adj)
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
