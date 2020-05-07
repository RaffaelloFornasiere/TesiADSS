#ifndef BIMAP_H
#define BIMAP_H
#include <unordered_map>

template <class A, class B>
class Bimap
{

public:
    Bimap();
    A operator[](const B& b){return map2[b];}
    B operator[](const A& a){return map1[a];}

    A at(const B& b){return map2.at(b);}
    B at(const A& a){return map1.at(a);}

private:
    std::unordered_map<A,B> map1;
    std::unordered_map<B,A> map2;
};

#endif // BIMAP_H
