#ifndef CIRCUITGRAPHINTERFACE_H
#define CIRCUITGRAPHINTERFACE_H
#include "../Graph/graph.h"
#include <unordered_map>
#include <iostream>
#include "../Circuit/circuit.h"



class CircuitGraphInterface;

enum class ParamType {CellRise, CellFall, FallTransition, RiseTransition};


class CircuitNode : public GraphNode<double>
{
public:
    virtual double GetInputPinCap() = 0;
    virtual double GetOutputPinCap() = 0;
    virtual double GetCellFall(double tIn, double outCap) = 0;
    virtual double GetCellRise(double tIn, double outCap) = 0;
    virtual double GetFallTransit(double tIn, double outCap) = 0;
    virtual double GetRiseTransit(double tIn, double outCap) = 0 ;

    virtual double Distance(GraphNode* a) override;
    virtual void PassFallTransit();
    virtual void PassRiseTransit();

protected:
    CircuitGraphInterface* it;
    double worstInRTransit;
    double worstOutRTransit;
    double distance;
};

class InputCircuitNode : public CircuitNode
{
    double GetInputPinCap() override;
    double GetOutputPinCap() override {return 0;};
    double GetCellFall(double tIn, double outCap) override;
    double GetCellRise(double tIn, double outCap) override;
    double GetFallTransit(double tIn, double outCap) override;
    double GetRiseTransit(double tIn, double outCap) override;

    double Distance(GraphNode* a) override;
private:
};



class CircuitGraphInterface
{
public:
    CircuitGraphInterface();

    double GetTimingParam(CircuitNode*, ParamType p);

    void Setup(); // for each cell, updates the output capacitance
    void CellSetup(Cell*); // update out cap for a signle cell

private:
    std::unordered_map<CircuitNode*, std::pair<Cell*, int>>  map1;
    std::unordered_map<Cell*, std::vector<CircuitNode*>> map2;

    Circuit* c;
};

#endif // CIRCUITGRAPHINTERFACE_H
