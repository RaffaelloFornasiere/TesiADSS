#ifndef CIRCUITGRAPHINTERFACE_H
#define CIRCUITGRAPHINTERFACE_H
#include "../Graph/graph.h"
#include <unordered_map>
#include <iostream>
#include "../Circuit/circuit.h"
#include "../BRKGA/BRKGA.h"
#include <thread>


class CircuitGraph;
enum class ParamType {CellRise, CellFall, FallTransition, RiseTransition};


// *************************************************************************************
// This class represent a node of the circuit. Each cell of the circuit is formed by
// a number of circuit nodes equal to the number of inputs plus the number of outputs.
// Each node has the task to store and calculate the delays.
// This class is the abstract class which is used by the solver. The following two classes
// inherit form this one and specialize the member methods to calcualte the delays
// *************************************************************************************


class CircuitNode : public GraphNode<double>
{
    friend std::ostream& operator<<(std::ostream& os, const CircuitNode& cn);

public:
    CircuitNode(CircuitGraph* it, double capacity, const std::string& name = "")
        : GraphNode<double>(name), it(it), outCapacity(capacity)
    {worstInRTransit = 0; worstOutRTransit = 0;}

    CircuitNode(CircuitGraph* it, const std::string& name)
        : CircuitNode(it, 0, name)
    {}

    CircuitNode(const CircuitNode& c)
        : CircuitNode(c.it, c.outCapacity, c.name)
    {}

    virtual ~CircuitNode() override {adj.clear();}

    CircuitNode& operator=(const CircuitNode& c) = delete;

    virtual double Distance(GraphNode* a) override;

    virtual void CalcOutputCap() = 0;
    virtual double GetWorstDelay() = 0;
    virtual double GetWorstTransition() = 0;

    std::string getName() const {return name;}
    void SetInTransition(double value) {if(value > worstInRTransit) worstInRTransit = value;}
    void SetCapacity(double value) {outCapacity = value;}


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
    InputCircuitNode(CircuitGraph *it, double capacity = 0, const std::string& name = "")
        : CircuitNode(it, capacity, name) {}
    InputCircuitNode(CircuitGraph* it, const std::string &name)
        : CircuitNode(it, name){}

    ~InputCircuitNode() override {}

    double GetWorstDelay() override {return 0;}
    double GetWorstTransition() override{return worstInRTransit;}
    void CalcOutputCap() override {outCapacity = 0;}

private:
};


class OutputCircuitNode : public CircuitNode
{
public:
    OutputCircuitNode(CircuitGraph* it, double capacity, const std::string& name = "")
        : CircuitNode(it, capacity, name)
    {delayUpdated = transitionUpdated = false;}
    OutputCircuitNode(CircuitGraph* it, const std::string& name)
        : CircuitNode(it, name)
    {delayUpdated = transitionUpdated = false;}
    OutputCircuitNode(const OutputCircuitNode& o)
        : CircuitNode(o)
    {delayUpdated = transitionUpdated = 0;}
    ~OutputCircuitNode() override {}

    double GetWorstDelay() override;
    double GetWorstTransition() override;
    void CalcOutputCap() override;

private:
    bool delayUpdated;
    bool transitionUpdated;
};



// ************************************************************************************
// the class provides the solver for the circuit delay problem
// it ihnerits from the two interfaces graph and BRKGA. this solver
// makes them coperate to solve the problem.
// The Graph provides the data structure which represent the circuit
// so the solver then can call TopologicalSort and GetWorstPathDistance
// in oreder to compute the delay of the circuit.
// The BRKGA provides the main function Evolve(). All the genetic algorithm
// stuff is performed inside BRKGA. According to the BRKGA guide, only the
// method Decode() should be overrided, but, due to keep interface more
// portable and because i added additional features to the original version
// of BRKGA, 2 others hotspots were added. One is the StopCriteria() an the other
// is Percentile() which is used to perform population resiszing to make
// the solution converge faster.
// ************************************************************************************





class CircuitGraph : public Graph<double>
{
    friend std::ostream& operator<<(std::ostream& os, const CircuitGraph& cg);
    friend class CircuitNode;


public:
    CircuitGraph(const Circuit &c);
    ~CircuitGraph();

    // circuit
    void SetupCaps(); // for each cell, updates the output capacitance
    void SetAllCells(std::vector<double> v);

    std::pair<const Cell*, int> GetCell(CircuitNode *n) const;
    double GetAreaOccupation() {return circuit.GetAreaOccupation();}

    size_t GetNumOfNodes() const {return map2.size();}


private:
    void CreateEdges();
    double GetTimingParam(CircuitNode*, ParamType p);
    void CellSetup(const Cell *cell); // update out cap for a signle cell


    //Cell {input_1, input_2 ... input_n, output_1 ... output_n}
    std::unordered_map<CircuitNode*, std::pair<const Cell*, int>>  map1;
    std::unordered_map<const Cell*, std::vector<CircuitNode*>> map2;
    const Circuit& circuit;
};

#endif // CIRCUITGRAPHINTERFACE_H
