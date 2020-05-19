#include "circuitgraphinterface.h"




std::ostream& operator<<(std::ostream &os, const CircuitGraph &cg)
{
    for(auto& i : cg.map2)
    {

        size_t inputs = i.first->GetNumOfInputs();
        if(inputs > 0)
        {
            os << "@begin" << std::endl;
            for(size_t j = 0; j < inputs; j++)
            {
                os << *i.second[j];// << std::endl;
            }
            os << "@end" << std::endl;
        }
        for(size_t j = 0; j < i.first->GetNumOfOutputs(); j++)
        {
            os << *i.second[inputs+j]; //<< std::endl;
        }
        //os << std::endl;
    }
    return os;
}

CircuitGraph::CircuitGraph(const Circuit *c)
    : circuit(c)
{
    // for each cell in the circuit, it will create some circuit nodes

    for(size_t i = 0; i < c->GetNumOfCells(); i++)
    {
        const Cell* aux = c->GetCell(i);

        std::vector<CircuitNode*> vect;
        size_t inputs = aux->GetNumOfInputs();
        size_t outputs = aux->GetNumOfOutputs();

        //create the input circuitnodes
        for(size_t j = 0; j < inputs; j++)
        {
            vect.emplace_back(new InputCircuitNode(this, "in" + std::to_string(j) + "_" + aux->getName()));
            map1.insert(std::make_pair(vect.back(), std::make_pair(aux, j)));
        }

        //create the output circuitnodes
        for(size_t j = 0; j < outputs; j++)
        {
            vect.emplace_back(new OutputCircuitNode(this, "out"+std::to_string(j)+"_"+aux->getName()));
            map1.insert(std::make_pair(vect.back(), std::make_pair(aux, inputs + j)));
        }
        if(vect.size() != 0)
            map2.insert(std::make_pair(aux, vect));

        //creates edges among the circuits nodes of this cell
        for(size_t j = 0; j < inputs; j++)
        {
            for(size_t k = 0; k < outputs; k++)
            {
                vect[j]->AddNeighbor(vect[inputs+k]);
            }
        }
    }

    CreateEdges();

    for(auto& i : map1)
    {
        adjList.push_back(i.first);
    }

    return;
}

void CircuitGraph::Setup()
{
    for(auto& x : map2)
    {
        circuit->ChangeCell(x.first);
        for(auto& y : x.second)
        {
            y->CalcOutputCap();
        }
    }

    return;
}

std::pair<const Cell *, int> CircuitGraph::GetCell(CircuitNode *n) const
{
    auto it = map1.begin();
    map1.find(n);

    if((it = map1.find(n)) == map1.end())
       throw std::invalid_argument("nessuna corrispondenza");

    auto it2 = *it;
    return it2.second;
}


void CircuitGraph::CreateEdges()
{
    for(size_t i = 0; i < circuit->GetNumOfCells(); i++)
    {
        const Cell* aux = circuit->GetCell(i);
        std::vector<const Cell*> outputs = circuit->GetOutputsOfCell(i);

        for(auto& o : outputs)
        {
            for(size_t k = 0; k < o->GetNumOfInputs(); k++)
            {
                for(size_t j = 0; j < aux->GetNumOfOutputs(); j++)
                {
                    if(aux->GetOutputName(j) == o->GetInputName(k))
                    {
                        CircuitNode* out    = map2[aux][aux->GetNumOfInputs()+j];
                        CircuitNode* in     = map2[o][k];
                        out->AddNeighbor(in);
                    }
                }
            }
        }
    }
}

InputCircuitNode::InputCircuitNode(CircuitGraph* it, int capacity)
    : CircuitNode(it, capacity)
{}

double InputCircuitNode::GetWorstDelay()
{
    return 0;
}

double InputCircuitNode::GetWorstTransition()
{
    return worstInRTransit;
}

void InputCircuitNode::CalcOutputCap()
{
    outCapacity = 0;
}

OutputCircuitNode::OutputCircuitNode(CircuitGraph* it, int capacity)
    :CircuitNode(it, capacity)
{}

double OutputCircuitNode::GetWorstDelay()
{
    auto p = it->GetCell(this);

    size_t outputIndex = p.second - p.first->GetNumOfInputs();

    double cellRise = p.first->GetTimingInfo(worstInRTransit, outCapacity, outputIndex, Cell::cell_rise);
    double cellFall = p.first->GetTimingInfo(worstInRTransit, outCapacity, outputIndex, Cell::cell_fall);

    return std::max(cellFall, cellRise);
}

double OutputCircuitNode::GetWorstTransition()
{
    auto p = it->GetCell(this);
    size_t outputIndex = p.second - p.first->GetNumOfInputs();

    double riseTransit = p.first->GetTimingInfo(worstInRTransit, outCapacity, outputIndex, Cell::rise_transition);
    double fallTransit = p.first->GetTimingInfo(worstInRTransit, outCapacity, outputIndex, Cell::fall_transition);

    return std::max(fallTransit, riseTransit);
}

void OutputCircuitNode::CalcOutputCap()
{
    auto p = it->GetCell(this);
    int index = p.second-p.first->GetNumOfInputs();

    outCapacity = p.first->GetOutPinCapacity(index);
    for(auto& i : adj)
    {
        auto p2 = it->GetCell(static_cast<CircuitNode*>(i));
        outCapacity += p2.first->GetInPinCapacity(p2.second);
    }
}

//std::ostream& operator<<(std::ostream &os, const CircuitNode &cn)
//{
//    //os << "\"" << cn.name << "\"";
//    for(auto i : cn.adj)

//        os << "\"" << cn.name << "\""
//           << "->\"" << i->getName() << "\""

//           << "|" << cn.outCapacity << "|"
//           << "|" << cn.outCapacity << "|"
//           << "|" << cn.outCapacity << "|"

//           << std::endl;
//    return  os;
//}

CircuitNode::CircuitNode(CircuitGraph *it, int capacity)
    : it(it), outCapacity(capacity)
{

}

double CircuitNode::Distance(GraphNode *a)
{
    double worstTransition = GetWorstTransition();

    CircuitNode* aux = static_cast<CircuitNode*>(a);  
    aux->SetInTransition(worstTransition);

    return GetWorstDelay();
}


