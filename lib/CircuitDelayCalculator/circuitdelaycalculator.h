#ifndef CIRCUITDELAYCALCULATOR_H
#define CIRCUITDELAYCALCULATOR_H
#include "../Circuit/circuit.h"
#include "../Graph/graph.h"




class GraphCell
{

private:
    double i;
};


class CircuitGraphNode : public GraphNode<Cell, double>
{

};


class CircuitGraph: public Graph<Cell, double>
{
public:
    CircuitGraph();


private:


};

#endif // CIRCUITDELAYCALCULATOR_H
