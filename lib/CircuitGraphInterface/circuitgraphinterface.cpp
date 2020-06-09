#include "circuitgraphinterface.h"
// *********************************************************************************************************************************************
//
// *********************************************************************************************************************************************
double OutputCircuitNode::GetWorstDelay()
{
    if(delayUpdated == false)
    {
        auto p = it->GetCell(this);

        size_t outputIndex = p.second - p.first->GetNumOfInputs();

        //        std::cout << "cell: "   << p.first->getName()
        //                  << " ctype: " << p.first->getType()
        //                  << " wit: "   << worstInRTransit
        //                  << " out_c: " << outCapacity << std::endl;

        double cellRise = p.first->GetTimingInfo(worstInRTransit, outCapacity, outputIndex, Cell::cell_rise);
        double cellFall = p.first->GetTimingInfo(worstInRTransit, outCapacity, outputIndex, Cell::cell_fall);
        delay = std::max(cellFall, cellRise);
        delayUpdated = true;
    }
    return delay;
}

double OutputCircuitNode::GetWorstTransition()
{
    if(transitionUpdated == false)
    {
        auto p = it->GetCell(this);
        size_t outputIndex = p.second - p.first->GetNumOfInputs();

        //        std::cout << "\tcell: "   << p.first->getName()
        //                  << " ctype: " << p.first->getType()
        //                  << " wit: "   << worstInRTransit
        //                  << " out_c: " << outCapacity << std::endl;


        double riseTransit = p.first->GetTimingInfo(worstInRTransit, outCapacity, outputIndex, Cell::rise_transition);
        double fallTransit = p.first->GetTimingInfo(worstInRTransit, outCapacity, outputIndex, Cell::fall_transition);
        worstOutRTransit = std::max(fallTransit, riseTransit);
        transitionUpdated = true;
    }

    return worstOutRTransit;
}

void OutputCircuitNode::CalcOutputCap()
{
    auto p = it->GetCell(this);
    int index = p.second-p.first->GetNumOfInputs();

    outCapacity = p.first->GetOutPinCapacity(index);
    //std::cout << "outcap of: " << p.first->getName() << std::endl;

    for(auto& i : adj)
    {
        auto p2 = it->GetCell(static_cast<CircuitNode*>(i));
        //std::cout << "\t cell: " << p2.first->getName();
        double cap = p2.first->GetInPinCapacity(p2.second);
        outCapacity += cap;
        //std::cout << " cap: " << cap << std::endl;
    }
    //std::cout << "\t total out_cap " << outCapacity << std::endl;
    transitionUpdated = false;
    delayUpdated = false;
}


double CircuitNode::Distance(GraphNode *a)
{

    double worstTransition = GetWorstTransition();

    CircuitNode* aux = static_cast<CircuitNode*>(a);
    aux->SetInTransition(worstTransition);

    return GetWorstDelay();
}
// *********************************************************************************************************************************************
//
// *********************************************************************************************************************************************



// *********************************************************************************************************************************************
//
// *********************************************************************************************************************************************
std::ostream& operator<<(std::ostream &os, const CircuitSolver &cg)
{
    std::vector<CircuitNode* > v;
    for(auto& i : cg.map1)
    {
        CircuitNode* c = i.first;
        v.emplace_back(c);

        //        size_t inputs = i.first->GetNumOfInputs();
        //        if(inputs > 0)
        //        {
        //            //os << "@begin" << std::endl;
        //            for(size_t j = 0; j < inputs; j++)
        //            {
        //                os << *i.second[j];// << std::endl;
        //            }
        //            //os << "@end" << std::endl;
        //        }
        //        for(size_t j = 0; j < i.first->GetNumOfOutputs(); j++)
        //        {
        //            os << *i.second[inputs+j]; //<< std::endl;
        //        }
        //        os << std::endl;
    }

    std::sort(v.begin(), v.end(),
              [&](CircuitNode* c1, CircuitNode* c2)
    {return c1->getName() > c2->getName();});

    for(auto& i : v)
        os << *i;

    return os;
}

CircuitSolver::CircuitSolver(const Circuit& c, BRKGAParams params, bool init)
    : BRKGA(c, params)
{
    // for each cell in the circuit, it will create some circuit nodes
    map1.clear();
    map2.clear();

    int in = 0, out = 0;
    for(size_t i = 0; i < input.GetNumOfCells(); i++)
    {
        const Cell* aux = input.GetCell(i);

        std::vector<CircuitNode*> vect;
        size_t inputs = aux->GetNumOfInputs();
        size_t outputs = aux->GetNumOfOutputs();

       // std::cout << aux->getName() << " " << inputs << " " << outputs << std::endl;

        //create the input circuitnodes
        for(size_t j = 0; j < inputs; j++)
        {
            if(!init)
                input.ChangeCell(i, 0);
            double cap = aux->GetInPinCapacity(j);
            vect.emplace_back(new InputCircuitNode(this, cap, "in" + std::to_string(in)));
            //std::cout << vect.back()->getName() << std::endl;
            map1.insert(std::make_pair(vect.back(), std::make_pair(aux, j)));
            in++;
        }

        //create the output circuitnodes
        for(size_t j = 0; j < outputs; j++)
        {
            if(!init)
                input.ChangeCell(i, 0);
            double cap = aux->GetInPinCapacity(j);
            vect.emplace_back(new OutputCircuitNode(this, cap, "out"+std::to_string(out)));
            //std::cout << vect.back()->getName() << std::endl;
            map1.insert(std::make_pair(vect.back(), std::make_pair(aux, inputs + j)));
            out++;
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

    TopologicalSort();

    Setup();

    return;
}

CircuitSolver::~CircuitSolver()
{
    for(auto& i : map1)
    {
        delete i.first;
    }
    map1.clear();
    map2.clear();
}

void CircuitSolver::Setup()
{
    for(auto& x : map2)
    {
        for(auto& y : x.second)
        {
            y->CalcOutputCap();
        }
    }

    return;
}

std::pair<const Cell *, int> CircuitSolver::GetCell(CircuitNode *n) const
{
    auto it = map1.begin();
    map1.find(n);

    if((it = map1.find(n)) == map1.end())
        throw std::invalid_argument("nessuna corrispondenza");

    auto it2 = *it;
    return it2.second;
}

void CircuitSolver::CreateEdges()
{
    for(size_t i = 0; i < input.GetNumOfCells(); i++)
    {
        const Cell* aux = input.GetCell(i);
        std::vector<const Cell*> outputs = input.GetOutputsOfCell(i);

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

void CircuitSolver::CellSetup(const Cell* cell)
{
    auto x = map2[cell];
    for(auto& y : x)
    {
        y->CalcOutputCap();
    }
}







std::ostream& operator<<(std::ostream &os, const CircuitNode &cn)
{
    os << "\"" << cn.name << "\"";
    for(auto i : cn.adj)
    {
        std::string s = i->getName();
        os /*<< "\"" << cn.name << "\""*/
                << "->\"" << i->getName() << "\"";
    }
    return  os;
}




// *********************************************************************************************************************************************
//                                                   BRKGA PART
// *********************************************************************************************************************************************
void CircuitSolver::Decode()
{
    //for each element inside the fitvect i'll take

    for(auto& item : fitVect)
    {
        //time_t start = clock();
        std::vector<double> alleles = hypercube[item.second];

        //        for(auto& i : alleles)
        //            std::cout << i << " ";

        for(size_t i = 0; i < input.GetNumOfCells(); i++)
        {
            input.ChangeCell(i, alleles[i]);
        }
        Setup();
        item.first = fit(getWorstPathDistance());
        //std::cout << " res: " << item.first << std::endl;
    }
}


bool CircuitSolver::StopCriteria()
{

    //std::cout << BestSolution() << std::endl;
    if(fitVect.front().first > lastBest)
    {
        lastBest = fitVect.front().first;
        counter = 0;
    }
    else
    {
        counter++;
    }
    if(counter > 5)
        return true;
    return  false;
    //return generations >= maxGenerations;
}

double CircuitSolver::Percentile()
{
    return std::floor(static_cast<double>(generations)/maxGenerations*100);
}

double CircuitSolver::BestFitness() const
{
    return fit_1(fitVect.front().first);
}


