#ifndef CIRCUITGRAPHINTERFACE_H
#define CIRCUITGRAPHINTERFACE_H
#include "../Graph/graph.h"
#include <unordered_map>
#include <iostream>
#include "../Circuit/circuit.h"


class CircuitGraph;

enum class ParamType {CellRise, CellFall, FallTransition, RiseTransition};


class CircuitNode : public GraphNode<double>
{
    //friend std::ostream& operator<<(std::ostream& os, const CircuitNode& cn);
public:
    CircuitNode(CircuitGraph* it, int capacity)
        : it(it), outCapacity(capacity){}

    CircuitNode(CircuitGraph* it, std::string name) : GraphNode<double>(name), it(it) {worstInRTransit = 0; worstOutRTransit = 0;}
    virtual ~CircuitNode(){}


    virtual double Distance(GraphNode* a) override;

    std::string getName() const {return name;}

    void SetInTransition(double value) {if(value > worstInRTransit) worstInRTransit = value;}

    void SetCapacity(double value){outCapacity = value;}

    //void AddNeighbor(GraphNode<double> *a) override;
    virtual void CalcOutputCap() = 0;

    virtual double GetWorstDelay() = 0; //{return delay;}
    virtual double GetWorstTransition() = 0; //{return worstOutRTransit;}


protected:
    CircuitGraph* it;
    double worstInRTransit;
    double worstOutRTransit;
    double delay;

    double outCapacity;
};




class InputCircuitNode : public CircuitNode
{
public:
    InputCircuitNode(CircuitGraph *it, int capacity = 0);
    InputCircuitNode(CircuitGraph* it, std::string name) : CircuitNode(it, name){}
    ~InputCircuitNode() {}

    double GetWorstDelay() override;
    double GetWorstTransition() override{return worstInRTransit;}

    void CalcOutputCap() override;

private:

};


class OutputCircuitNode : public CircuitNode
{

public:
    OutputCircuitNode(CircuitGraph* it, int capacity)
        :CircuitNode(it, capacity){}
    OutputCircuitNode(CircuitGraph* it, std::string name):CircuitNode(it, name){}
    ~OutputCircuitNode(){}

    double GetWorstDelay() override;
    double GetWorstTransition() override;
    void CalcOutputCap() override;

private:

};



class CircuitGraph : public Graph<double>
{
    friend std::ostream& operator<<(std::ostream& os, const CircuitGraph& cg);
    friend class CircuitNode;

public:
    CircuitGraph(const Circuit* c);
    ~CircuitGraph();

    double GetTimingParam(CircuitNode*, ParamType p);

    void Setup(); // for each cell, updates the output capacitance
    void CellSetup(Cell*); // update out cap for a signle cell
    void ChangeCell(const Cell* cell) const {circuit->ChangeCell(cell);}
    std::pair<const Cell*, int> GetCell(CircuitNode *n) const;

private:
    void CreateEdges();

    //Cell {input_1, input_2 ... input_n, output_1 ... output_n}
    std::unordered_map<CircuitNode*, std::pair<const Cell*, int>>  map1;
    std::unordered_map<const Cell*, std::vector<CircuitNode*>> map2;

    const Circuit* circuit;
};

#endif // CIRCUITGRAPHINTERFACE_H
