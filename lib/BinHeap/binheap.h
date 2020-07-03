#ifndef BINARYHEAP_H
#define BINARYHEAP_H
#include <iostream>
#include <climits>
#include <vector>
#include <unordered_map>
#include <functional>


//  *****************************************************************************
//  Provides a binary heap template used to construct priority queues inside graphs.
//  The idea behind the strucure is to use array based heap-structure that stores
//  indices and object pointers inside a map in order to uniquely identify each
//  object inside the data structure with a pointer that it's also used
//  outside this class.
//  These pointers identify the object inside and outside the template, so every
//  operation is referred to a pointer.
//  To avoid the O(n) cost to find the pointer position in the vector, an unordered_map
//  is used to keep track of the index inside the vector. This deletes the O(n) search
//  cost but intruduce a bit overhead due to keep update the map.
//  The class also provides a method to select if the heap is a minheap or a maxheap
//  and also can take a custom comp function which will be used to order the heap
//  *****************************************************************************

template<class T, class K> class BinHeap;

template <class T, class K>
class BinHeapNode
{
public:
    BinHeapNode(T item = T(), K key = K()):item(item), key(key) {}
    ~BinHeapNode(){}
/*
    K getKey() const {return key;}
    void setKey(const K &value) {key = value;}
    T getItem() const {return item;}
    void setItem(const T &value) {item = value;}
*/

private:
    friend class BinHeap<T,K>;
    T item;
    K key;
};


template <class T, class K>
class BinHeap
{
public:
    BinHeap(int reserve = 10, const std::function<bool(K, K)>& compare = [](K k1, K k2)
    {return k1 > k2;});

    std::pair<T,K> Pop();
    bool DecreaseKey(BinHeapNode<T,K>* n, K newKey);
    bool IncreaseKey(BinHeapNode<T,K>* n, K newKey);
    bool Empty() const {return vect.size() == 0;}
    bool Contains(BinHeapNode<T,K>* n) const;
    bool DeleteElement(BinHeapNode<T,K>* n);

    std::pair<T,K> Front() const;
    BinHeapNode<T,K>* Push(const T& item, K key);

    bool CheckCollisions();
    void SetCompare(const std::function<bool(K, K)> &value);

    inline static const std::function<bool(K, K)> minHeap = [](K k1, K k2){return k1 > k2;};
    inline static const std::function<bool(K, K)> maxHeap = [](K k1, K k2){return k1 < k2;};

private:
    int parent(int i) const  { return (i-1)/2; }
    int left(int i) const { return (2*i + 1); }
    int right(int i) const { return (2*i + 2); }
    void Heapify(size_t i);

    bool Comparator();
    std::function<bool(K, K)> compare;
    std::vector<BinHeapNode<T,K>*> vect;
    std::unordered_map<BinHeapNode<T,K>*, int> pos;
};
#endif // BINARYHEAP_H


template<class T, class K>
BinHeap<T,K>::BinHeap(int reserve, const std::function<bool(K, K)>& compare)
    : compare(compare)
{
    vect.reserve(reserve);
    pos.reserve(reserve);
}

template<class T, class K>
std::pair<T,K> BinHeap<T,K>::Pop()
{
    std::pair<T,K> res = std::make_pair(vect.front()->item, vect.front()->key);
    BinHeapNode<T,K>* ptr = vect.front();
    pos.erase(ptr);
    vect.front() = vect.back();
    delete ptr;
    ptr = nullptr;
    vect.pop_back();
    Heapify(0);
    return res;
}


template<class T, class K>
BinHeapNode<T, K> *BinHeap<T, K>::Push(const T &item, K key)
{
    vect.emplace_back(new BinHeapNode<T,K>(item, key));
    int i = vect.size()-1;

    std::pair<BinHeapNode<T,K>*, int> aux(vect[i], i);
    pos.insert(aux);
    while(i > 0 && compare(vect[parent(i)]->key, vect[i]->key))
    {
        std::swap(vect[i], vect[parent(i)]);
        std::swap(pos.at(vect[i]), pos.at(vect[parent(i)]));
        i = parent(i);
    }

    return vect[i];
}

template<class T, class K>
bool BinHeap<T,K>::CheckCollisions()
{
    for(size_t i = 0; i < pos.size(); i++)
    {
        size_t count = pos.bucket_size(i);
        if(count > 0)
        std::cout << "bucket:size " << count << std::endl;
    }
    return 0;
}

template<class T, class K>
void BinHeap<T,K>::Heapify(size_t i)
{
    size_t r = right(i);
    size_t l = left(i);
    size_t best = i;
    if(l < vect.size() && compare(vect[i]->key, vect[l]->key))
        best = l;
    if(r < vect.size() && compare(vect[best]->key, vect[r]->key))
        best = r;
    if(best != i)
    {
        std::swap(vect[i], vect[best]);
        std::swap(pos.at(vect[i]), pos.at(vect[best]));
        Heapify(best);
    }

}

template<class T, class K>
void BinHeap<T,K>::SetCompare(const std::function<bool (K, K)> &value)
{
    compare = value;
}

template<class T, class K>
bool BinHeap<T,K>::DecreaseKey(BinHeapNode<T,K> *n, K newKey)
{
    auto it = pos.begin();
    if((it = pos.find(n)) == pos.end())
        return 0;
    n->key -= newKey;
    int i = (*it).second;
    Heapify(i);
    while(i > 0 && compare(vect[parent(i)]->key, vect[i]->key))
    {
        std::swap(vect[i], vect[parent(i)]);
        std::swap(pos.at(vect[i]), pos.at(vect[parent(i)]));
        i = parent(i);
    }
    return 1;
}

template<class T, class K>
bool BinHeap<T,K>::IncreaseKey(BinHeapNode<T, K> *n, K newKey)
{
    auto it = pos.begin();
    if((it = pos.find(n)) == pos.end())
        return 0;
    n->key += newKey;
    int i = (*it).second;
    Heapify(i);
    while(i > 0 && compare(vect[parent(i)]->key, vect[i]->key))
    {
        std::swap(vect[i], vect[parent(i)]);
        std::swap(pos.at(vect[i]), pos.at(vect[parent(i)]));
        i = parent(i);
    }
    return 1;
}

template<class T, class K>
bool BinHeap<T,K>::Contains(BinHeapNode<T, K> *n) const
{
    return pos.find(n) != pos.end();
}

template<class T, class K>
std::pair<T, K> BinHeap<T,K>::Front() const
{
    return std::make_pair(vect[0]->item, vect[0]->key);
}

template<class T, class K>
bool BinHeap<T,K>::DeleteElement(BinHeapNode<T, K> *n)
{
    bool res = DecreaseKey(n, vect[0].key+1);
    if(res)
        Pop();
    return res;
}

