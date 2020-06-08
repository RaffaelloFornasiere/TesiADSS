#include <iostream>
#include <vector>
#include <../../../lib/BinHeap/binheap.h>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <conio.h>

class Item
{
    friend std::ostream& operator<<(std::ostream& os, const Item& i)
    {
        os << "a:\t" << i.a;
        return os;
    }
public:
    Item(int a): a(a){}//, b(b) {}
    int a;
    //double b;
};

int main()
{

    int n = 10;
    int k = 100;
    srand(time(nullptr));
    for(int i = 1; i < 2; i++)
    {
        n = pow(10, i);
        BinHeap<Item, double> heap(n, BinHeap<Item,double>::minHeap);
        std::vector<double> v; v.reserve(n);
        std::vector<BinHeapNode<Item, double>*> ptrs; ptrs.reserve(n);

        time_t start = clock();
        for(int i = 0; i < n; i++)
        {
            double d = rand()%k;
            v.emplace_back(d);
            ptrs.emplace_back(heap.Push(Item(i), d));
        }
        heap.DecreaseKey(ptrs[7], ptrs[7]->getKey()-1);
        heap.IncreaseKey(ptrs[0], 1000 - ptrs[0]->getKey());

        while(!heap.Empty())
        {
            auto x = heap.Pop();
            std::cout << "item:\t" << x.first << "\tkey:\t" << x.second <<std::endl;
        }
        std::cout << std::setw(10) << n << std::setw(10) << "b" << std::endl;

        std::cout<< "sorting: " << std::setw(10) << n <<  std::setw(5) << "\telements in O(nlog(n)) required: " << std::setw(8) <<  clock() - start << " [ms] " << std::endl;
    }

    return 0;
}
