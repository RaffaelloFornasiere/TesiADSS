#ifndef BINARYHEAP_H
#define BINARYHEAP_H
#include <iostream>
#include <climits>
#include <vector>
#include <unordered_map>
#include <functional>

template<class T, class K> class BinHeap;

template <class T, class K>
class BinHeapNode
{
    //BinHeapNode& operator=(const BinHeapNode& b) {this->item = }
public:
    BinHeapNode(T item = T(), K key = K()):item(item), key(key) {/*std::cout << "BinHeapNode constructor" << std::endl;*/}
    ~BinHeapNode(){/*std::cout << "BinHeapNode destructor" << std::endl;*/}
    K getKey() const {return key;}
    void setKey(const K &value) {key = value;}

    T getItem() const {return item;}
    void setItem(const T &value) {item = value;}

private:
    friend class BinHeap<T,K>;
    T item;
    K key;
};


template <class T, class K>
class BinHeap
{
public:
    BinHeap(int reserve = 10, const std::function<bool(K, K)>& compare = [](K k1, K k2){return k1 > k2;});

    std::pair<T,K> Pop();
    bool DecreaseKey(BinHeapNode<T,K>* n, K newKey);
    bool IncreaseKey(BinHeapNode<T,K>* n, K newKey);
    //bool ChangeKey(BinHeapNode<T,K>* n, K newKey);
    std::pair<T,K> Front();
    void DeleteElement(BinHeapNode<T,K>* n);
    BinHeapNode<T,K>* Push(T item, K key);
    bool Empty() const {return vect.size() == 0;}
    bool Contains(BinHeapNode<T,K>* n) {return pos.end() != pos.find(n);}

    void SetCompare(const std::function<bool(K, K)> &value);

    inline static const std::function<bool(K, K)> minHeap = [](K k1, K k2){return k1 > k2;};
    inline static const std::function<bool(K, K)> maxHeap = [](K k1, K k2){return k1 < k2;};

private:
    // to get index of parent of node at index i
    int parent(int i) { return (i-1)/2; }

    // to get index of left child of node at index i
    int left(int i) { return (2*i + 1); }

    // to get index of right child of node at index i
    int right(int i) { return (2*i + 2); }
    void Heapify(size_t i);

    bool Comparator();
    std::function<bool(K, K)> compare;

    std::unordered_map<BinHeapNode<T,K>*, int> pos;
    std::vector<BinHeapNode<T,K>*> vect;
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
    BinHeapNode<T,K> aux = *vect.front();

    vect.front() = vect.back();
    vect.pop_back();
    Heapify(0);
    return std::make_pair(aux.item, aux.key);
}


template<class T, class K>
BinHeapNode<T, K> *BinHeap<T, K>::Push(T item, K key)
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

//template<class T, class K>
//bool BinHeap<T,K>::ChangeKey(BinHeapNode<T, K> *n, K newKey)
//{
//    if(n && compare(n->key, newKey) )
//        return DecreaseKey(n, abs(n->key - newKey));
//    else if (n && n->key < newKey)
//        return IncreaseKey(n, abs(newKey - n->key));

//    return false;

//}

template<class T, class K>
std::pair<T, K> BinHeap<T,K>::Front()
{
    return std::make_pair(vect[0]->item, vect[0]->key);
}

template<class T, class K>
void BinHeap<T,K>::DeleteElement(BinHeapNode<T, K> *n)
{
    DecreaseKey(n, vect[0].key+1);
    Pop();
}



//#ifndef BINARYHEAP_H
//#define BINARYHEAP_H
//#include <iostream>
//#include <climits>
//#include <vector>
//#include <unordered_map>
//#include <functional>

//template<class T, class K> class BinHeap;

//template <class T, class K>
//class BinHeapNode
//{
//    //BinHeapNode& operator=(const BinHeapNode& b) {this->item = }
//public:
//    BinHeapNode(T item = T(), K key = K()):item(item), key(key) {}

//private:
//    friend class BinHeap<T,K>;
//    T item;
//    K key;
//};


//template <class T, class K>
//class BinHeap
//{
//public:
//    BinHeap(int reserve = 10);

//    std::pair<T,K> Pop();
//    bool DecreaseKey(BinHeapNode<T,K>* n, K newKey);
//    bool IncreaseKey(BinHeapNode<T,K>* n, K newKey);
//    bool ChangeKey(BinHeapNode<T,K>* n, K newKey);
//    std::pair<T,K> Front();
//    void DeleteElement(BinHeapNode<T,K>* n);
//    BinHeapNode<T,K>* Push(T item, K key);
//    bool Empty() const {return vect.size() == 0;}
//    bool Contains(BinHeapNode<T,K>* n) {return pos.end() != pos.find(n);}

//private:
//    // to get index of parent of node at index i
//    int parent(int i) { return (i-1)/2; }

//    // to get index of left child of node at index i
//    int left(int i) { return (2*i + 1); }

//    // to get index of right child of node at index i
//    int right(int i) { return (2*i + 2); }
//    void Heapify(size_t i);

//    bool Comparator();
//    std::function<bool(K, K)> comparator;
//    std::unordered_map<BinHeapNode<T,K>*, int> pos;
//    std::vector<BinHeapNode<T,K>*> vect;
//};
//#endif // BINARYHEAP_H


//template<class T, class K>
//BinHeap<T,K>::BinHeap(int reserve)
//{
//    comparator = [](K k1, K k2){return k1 < k2;};
//    vect.reserve(reserve);
//    pos.reserve(reserve);
//}

//template<class T, class K>
//std::pair<T,K> BinHeap<T,K>::Pop()
//{
//    BinHeapNode<T,K> aux = *vect.front();
//    delete vect.front();
//    vect.front() = vect.back();
//    vect.pop_back();
//    Heapify(0);
//    return std::make_pair(aux.item, aux.key);
//}


//template<class T, class K>
//BinHeapNode<T, K> *BinHeap<T, K>::Push(T item, K key)
//{
//    vect.emplace_back(new BinHeapNode<T,K>(item, key));
//    int i = vect.size()-1;

//    std::pair<BinHeapNode<T,K>*, int> aux(vect[i], i);
//    pos.insert(aux);
//    while(i > 0 && vect[parent(i)]->key > vect[i]->key)
//    {
//        std::swap(vect[i], vect[parent(i)]);
//        std::swap(pos.at(vect[i]), pos.at(vect[parent(i)]));
//        i = parent(i);
//    }

//    return vect[i];
//}

//template<class T, class K>
//void BinHeap<T,K>::Heapify(size_t i)
//{
//    size_t r = right(i);
//    size_t l = left(i);
//    size_t smallest = i;
//    if(l < vect.size() && vect[l]->key < vect[i]->key)
//        smallest = l;
//    if(r < vect.size() && vect[r]->key < vect[smallest]->key)
//        smallest = r;
//    if(smallest != i)
//    {
//        std::swap(vect[i], vect[smallest]);
//        std::swap(pos.at(vect[i]), pos.at(vect[smallest]));
//        Heapify(smallest);
//    }

//}

//template<class T, class K>
//bool BinHeap<T,K>::DecreaseKey(BinHeapNode<T,K> *n, K newKey)
//{
//    auto it = pos.begin();
//    if((it = pos.find(n)) == pos.end())
//        return 0;
//    n->key -= newKey;
//    int i = (*it).second;
//    while(i > 0 && vect[parent(i)]->key > vect[i]->key)
//    {
//        std::swap(vect[i], vect[parent(i)]);
//        i = parent(i);
//    }
//    return 1;
//}

//template<class T, class K>
//bool BinHeap<T,K>::IncreaseKey(BinHeapNode<T, K> *n, K newKey)
//{
//    auto it = pos.begin();
//    if((it = pos.find(n)) == pos.end())
//        return 0;
//    n->key += newKey;
//    int i = (*it).second;
//    Heapify(i);
//    return 1;
//}

//template<class T, class K>
//bool BinHeap<T,K>::ChangeKey(BinHeapNode<T, K> *n, K newKey)
//{
//    if(n && n->key > newKey)
//        return DecreaseKey(n, n->key - newKey);
//    else if (n && n->key < newKey)
//        return IncreaseKey(n, newKey-n->key);

//    return false;

//}

//template<class T, class K>
//std::pair<T, K> BinHeap<T,K>::Front()
//{
//    return std::make_pair(vect[0]->item, vect[0]->key);
//}

//template<class T, class K>
//void BinHeap<T,K>::DeleteElement(BinHeapNode<T, K> *n)
//{
//    DecreaseKey(n, vect[0].key+1);
//    Pop();
//}









