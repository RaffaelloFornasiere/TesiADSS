#ifndef CIRCUITGRAPHINTERFACE_H
#define CIRCUITGRAPHINTERFACE_H
#include "../Graph/graph.h"
#include <unordered_map>
#include <iostream>
#include "../Circuit/circuit.h"
#include "../BRKGA/BRKGA.h"
#include <thread>


class CircuitSolver;
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
    CircuitNode(CircuitSolver* it, int capacity, std::string name = "")
        : GraphNode<double>(name), it(it), outCapacity(capacity)
    {worstInRTransit = 0; worstOutRTransit = 0;}

    CircuitNode(CircuitSolver* it, std::string name)
        : GraphNode<double>(name), it(it)
    {worstInRTransit = 0; worstOutRTransit = 0;}
    virtual ~CircuitNode(){}


    virtual double Distance(GraphNode* a) override;

    std::string getName() const {return name;}

    void SetInTransition(double value) {if(value > worstInRTransit) worstInRTransit = value;}

    void SetCapacity(double value) {outCapacity = value;}

    //void AddNeighbor(GraphNode<double> *a) override;
    virtual void CalcOutputCap() = 0;

    virtual double GetWorstDelay() = 0; //{return delay;}
    virtual double GetWorstTransition() = 0; //{return worstOutRTransit;}


protected:
    CircuitSolver* it;
    double worstInRTransit;
    double worstOutRTransit;
    double delay;

    double outCapacity;
};




class InputCircuitNode : public CircuitNode
{
public:
    InputCircuitNode(CircuitSolver *it, int capacity = 0, std::string name = "")
        : CircuitNode(it, capacity, name) {}
    InputCircuitNode(CircuitSolver* it, std::string name)
        : CircuitNode(it, name){}
    ~InputCircuitNode() {}

    double GetWorstDelay() override {return 0;}
    double GetWorstTransition() override{return worstInRTransit;}
    void CalcOutputCap() override {outCapacity = 0;}

private:
};


class OutputCircuitNode : public CircuitNode
{
public:
    OutputCircuitNode(CircuitSolver* it, int capacity, std::string name = "")
        : CircuitNode(it, capacity, name)
    {delayUpdated = transitionUpdated = false;}
    OutputCircuitNode(CircuitSolver* it, std::string name)
        : CircuitNode(it, name){}
    ~OutputCircuitNode(){}

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


class CircuitSolver : public Graph<double>, public BRKGA<Circuit, double>
{
    friend std::ostream& operator<<(std::ostream& os, const CircuitSolver& cg);
    friend class CircuitNode;


public:
    CircuitSolver(const Circuit* c, BRKGAParams params, bool init = 0);
    ~CircuitSolver();


    // circuit
    void Setup(); // for each cell, updates the output capacitance
    std::pair<const Cell*, int> GetCell(CircuitNode *n) const;
    void ChangeCell(const Cell* cell) const {circuit->ChangeCell(cell);}


    // *********** Solver methods **************
    void Decode() override;
    bool StopCriteria() override;
    double Percentile() override;
    double BestSolution() const override;
    //double BestSolution() const override {return fit_1(fitVect.front().first);}
    // *****************************************


    size_t getMaxGenerations() const {return maxGenerations;}
    void setMaxGenerations(const size_t &value){maxGenerations = value;}

    double GetAreaOccupation() {return circuit->GetAreaOccupation();}

private:

    void CreateEdges();
    double GetTimingParam(CircuitNode*, ParamType p);
    void CellSetup(const Cell *cell); // update out cap for a signle cell


    //Cell {input_1, input_2 ... input_n, output_1 ... output_n}
    std::unordered_map<CircuitNode*, std::pair<const Cell*, int>>  map1;
    std::unordered_map<const Cell*, std::vector<CircuitNode*>> map2;
    const Circuit* circuit;






    size_t maxGenerations;
    std::function<double(double)> fit = [](double time){return 100.0/time;};
    std::function<double(double)> fit_1 = [](double val){return 100.0/val;};


};

#endif // CIRCUITGRAPHINTERFACE_H
