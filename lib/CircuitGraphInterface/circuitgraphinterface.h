#ifndef CIRCUITGRAPHINTERFACE_H
#define CIRCUITGRAPHINTERFACE_H
#include "../Graph/graph.h"


class CircuitNode : public GraphNode<int, double>
{
public:
    virtual double GetInputPinCap() = 0;
    virtual double GetOutputPinCap() = 0;
    virtual double GetCellFall() = 0;
    virtual double GetCellRise(double tIn, double outCap);
    virtual double GetFallTransit(double tIn, double outCap);
    virtual double GetRiseTransit(double tIn, double outCap);
private:

};


class CircuitGraphInterface
{
public:
    CircuitGraphInterface();
};




class CircuitGraphInterface
{
public:
    CircuitGraphInterface();
};

#endif // CIRCUITGRAPHINTERFACE_H
