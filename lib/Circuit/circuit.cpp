#include "circuit.h"

// ***************************************************************************************************************************
//                                          CIRCUIT METHODS : INPUT PART
// ***************************************************************************************************************************

std::ostream& operator<<(std::ostream &os, const Circuit &c)
{
    for(size_t i = 0; i < c.inputLists.size(); i++)
    {
        c.inputLists[i].first->setId("id:"+std::to_string(i)+"_");
    }

    for(auto& x : c.adjList)
    {
        os << "\"" << x.first.getName() << "_" << x.first.getId() << "(" << x.first.getType() << ")" << "\"";
        for(auto& y : x.second)
        {
            if(y->getType() != "")
                os// <<"\"" << x.first.name << "(" << x.first.type << ")" << "\""
                        << "->" << "\""<< y->getName() << "_" << y->getId() << "(" << y->getType() << ")" << "\"";
            //<<std::endl;

        }
        os << std::endl;
    }
    return os;
}


std::istream& operator>>(std::istream &is, Circuit &c)
{
    std::string aux;

    while(std::getline(is>>std::ws, aux) && aux.find("begin") == std::string::npos);


    while(std::getline(is>>std::ws, aux) && aux.find("end") == std::string::npos)
    {
        std::regex pattern{R"***(\w+ +<= +\w\(\d+\) +\w+ \w\(\d+\);)***"};
        std::regex pattern2{R"***(([\w\(\)]* *<= *[\w\(\)]*;))***"};
        if(std::regex_match(aux, pattern))
            c.readInstruction1(aux);
        else if (aux.find("port map") != std::string::npos)
            c.readInstruction2(aux);
        else if(std::regex_match(aux, pattern2))
            c.readInstruction3(aux);
    }



    return is;
}


//search inside the adjacency list a cell that have "name" as output pin
std::list<std::pair<Cell, std::vector<Cell*>>>::iterator Circuit::searchOutputSingal(const std::string& name)
{
    return std::find_if(adjList.begin(),
                        adjList.end(),
                        [=](const std::pair<Cell, std::vector<Cell*>>& p)
    {
        return std::find(p.first.output.begin(), p.first.output.end(), name) != p.first.output.end();
    });
}

// returns the iterator of the type searched
std::vector<std::vector<Cell>>::const_iterator Circuit::searchCellType(const std::string& type)
{
    for(size_t i = 0; i < cellSelection->size(); i++)
    {
        if(cellSelection->at(i)[0].type == type)
            return cellSelection->begin()+i;
    }
    return cellSelection->end();
}

// p_0_0 <= A(0) and B(0);
bool Circuit::readInstruction1(std::string line)
{
    std::string name = "port" + std::to_string(adjList.size());
    adjList.emplace_back(std::make_pair(Cell(name), std::vector<Cell*>()));
    inputLists.emplace_back(std::make_pair(&adjList.back().first, std::vector<Cell*>()));

    //takes the name of the output
    std::stringstream ss (line);
    ss >> line;
    adjList.back().first.output.emplace_back(line);


    ss >> line; //skips the <=

    ss >> line;
    //cerca nella lista di adiacenza se c'è qualche cella che ha come uscita il nome del pin trovato come ingresso
    auto i = searchOutputSingal(line);

    adjList.back().first.input.emplace_back(line);
    if(i != adjList.end())
    {
        //aggiunge tale cella alla lista in input di questa cella e
        inputLists.back().second.push_back(&(*i).first);
        //adjList.back().first.input.emplace_back(line);
        //aggiunge alla lista di tale cella questa cella
        (*i).second.push_back(&adjList.back().first);
    }
    if(!(ss >> adjList.back().first.type))
    {
        adjList.pop_back();
        return false;
    }

    adjList.back().first.type += '2';
    std::transform(adjList.back().first.type.begin(),
                   adjList.back().first.type.end(),
                   adjList.back().first.type.begin(),
                   ::toupper);

#ifdef CIRCUIT_DEBUG
    std::cout << "instruction type 1";
    std::cout << std::setw(15) << "type: -" << adjList.back().first.type<<"-";
    std::cout << std::setw(25) << "output: " << "-"<<adjList.back().first.output[0].name<<"-";
    std::cout << std::setw(45) <<"input: ";
    std::cout << "-" << line<<"-";
#endif
    if(!(ss >> line))
    {
        adjList.pop_back();
        return false;
    }
    i = searchOutputSingal(line);

    adjList.back().first.input.emplace_back(line);
    if(i != adjList.end())
    {
        //aggiunge tale cella alla lista in input di questa cella e
        inputLists.back().second.push_back(&(*i).first);
        //adjList.back().first.input.emplace_back(line);
        //aggiunge alla
        (*i).second.push_back(&adjList.back().first);
    }

#ifdef CIRCUIT_DEBUG
    std::cout << "-" << line<<"-"<<std::endl;
#endif

    return true;
}


// 	U1 : OR2 port map (A1 => net_3, A2 => net_2, Z => net_8);
bool Circuit::readInstruction2(std::string line)
{
    // resize del vettore
    std::string name = "port" + std::to_string(adjList.size());
    adjList.emplace_back(std::make_pair(Cell(name), std::vector<Cell*>()));
    inputLists.emplace_back(std::make_pair(&adjList.back().first, std::vector<Cell*>()));


    auto f = line.find(":");
    auto e = line.find("port");
    adjList.back().first.type = line.substr(f + 1, e - f-1);

    if(adjList.back().first.type.find("INV") != std::string::npos)
        adjList.back().first.type = "NOT1";
    else
        adjList.back().first.type.erase(std::remove_if(adjList.back().first.type.begin(),
                                                       adjList.back().first.type.end(), isspace),
                                        adjList.back().first.type.end());


#ifdef CIRCUIT_DEBUG
    std::cout << "instruction type 2";
    std::cout << std::setw(20) << "type: -" << adjList.back().first.type<<"-";
#endif

    auto it = searchCellType(adjList.back().first.type);

    if(it == cellSelection->end())
    {

        adjList.pop_back();
        std::cerr << "the circuit contains cells not defined";
        return false;
        //throw std::invalid_argument("the circuit contains cells not defined");
    }

    line.erase(0, line.find("=>")+2);

#ifdef CIRCUIT_DEBUG
    std::cout << std::setw(20) << "inputs: ";
#endif


    for(size_t i = 0; i < (*it)[0].input.size(); i++)
    {
        std::string in;
        in = line.substr(0, line.find(","));
        in.erase(std::remove_if(in.begin(), in.end(), ::isspace), in.end());
        auto it2 = searchOutputSingal(in);

#ifdef CIRCUIT_DEBUG
        std::cout << "-"<<in<<"-";
#endif
        adjList.back().first.input.emplace_back(in);
        if(it2 != adjList.end())
        {
            //aggiunge tale cella alla lista in input di questa cella e
            inputLists.back().second.push_back(&(*it2).first);
            //aggiunge alla
            (*it2).second.push_back(&adjList.back().first);
        }
        line.erase(0, line.find("=>")+2);
    }
    for(size_t i = 0; i < (*it)[0].output.size(); i++)
    {
        std::string in;
        size_t j = 0;
        if((j = line.find(",")) != std::string::npos)
            in = line.substr(0, line.find(","));
        else
            in = line.substr(0, line.size()-2);

        in.erase(std::remove_if(in.begin(), in.end(), ::isspace), in.end());
        adjList.back().first.output.push_back(in);
        line.erase(0, line.find_first_of(">)")+1);

    }





#ifdef CIRCUIT_DEBUG
    std::cout <<  std::setw(20) << "out name:\t";
    for(auto i : adjList.back().first.output)
        std::cout << "-" << i.name << "-";
    std::cout << std::endl;
#endif
    return true;
}


//	p_0_0 <= A(0) and B(0);
bool Circuit::readInstruction3(std::string line)
{
    std::string name = "port" + std::to_string(adjList.size());
    adjList.emplace_back(std::make_pair(Cell(name), std::vector<Cell*>()));
    inputLists.emplace_back(std::make_pair(&adjList.back().first, std::vector<Cell*>()));


    std::string outName = line.substr(0, line.find("<="));
    outName.erase(std::remove_if(outName.begin(), outName.end(), isspace), outName.end());
    adjList.back().first.output.emplace_back(outName);

#ifdef CIRCUIT_DEBUG
    std::cout << "instruction type 3";
    std::cout << std::setw(20) << "out name: -" << outName << "-";
#endif

    adjList.back().first.type = "void";
    line = line.substr(line.find("=")+1, line.size()-line.find("=")-2);
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    auto i = searchOutputSingal(line);
    adjList.back().first.input.emplace_back(line);
    if(i != adjList.end())
    {
        //aggiunge tale cella alla lista in input di questa cella e
        inputLists.back().second.push_back(&(*i).first);
        //adjList.back().first.input.emplace_back(line);
        //aggiunge alla
        (*i).second.push_back(&adjList.back().first);
    }

#ifdef CIRCUIT_DEBUG
    std::cout <<  std::setw(20) << "type:\t" << "-void-";
    std::cout <<  std::setw(20) << "ingresso:\t" << "-"<<line<<"-";
    std::cout << std::endl;
#endif

    return true;
}






// ***************************************************************************************************************************
//                                          CIRCUIT METHODS : SERVICES PART
// ***************************************************************************************************************************


//actually non used. Only for debug
bool operator==(const Circuit &c1, const Circuit &c2)
{
    bool res1 = 1;
    for(size_t i = 0; i < c1.inputLists.size(); i++)
    {
        if((*c1.inputLists[i].first) != (*c2.inputLists[i].first))
        {
            return 0;
        }
        else if (c1.inputLists[i].second.size() != c2.inputLists[i].second.size())
        {
            return 0;
        }
        else
        {
            for(size_t j = 0; j < c1.inputLists[i].second.size(); j++)
            {
                if((*c1.inputLists[i].second[j]) != (*c2.inputLists[i].second[j]))
                {
                    return 0;
                }
            }
        }
    }
    bool res3 = (c1.cellSelection == c2.cellSelection);

    return res1 && res3;
}

void Circuit::AssignRandom()
{

    for (auto& it : adjList)
    {
        for(size_t i = 0; i < cellSelection->size(); i++)
        {
            if(cellSelection->at(i)[0].type == it.first.type)
            {
                int j = rand()%cellSelection->at(i).size();
                it.first.CopyParams(&cellSelection->at(i)[j]);
            }
        }
    }

    return;
}

void Circuit::AssignAll(double p) const
{
    for(size_t i = 0; i < inputLists.size(); i++)
        ChangeCell(i, p);
}

// returns all the cells as input of given cell
std::vector<const Cell *> Circuit::GetInputsOfCell(int cell) const
{
    std::vector<const Cell*> aux;
    aux.reserve(inputLists[cell].second.size());
    for(auto& i : inputLists[cell].second)
        aux.emplace_back(i);
    return aux;
}

//same as above but implemented wiht pointer search
std::vector<const Cell *> Circuit::GetInputsOfCell(const Cell *cell) const
{
    auto it = std::find_if(inputLists.begin(), inputLists.end(),
                           [=](auto& c)
    {return c.first == cell;});

    if(it == inputLists.end())
        throw std::invalid_argument("no matching cell in circuit");

    std::vector<const Cell*> res;
    res.reserve((*it).second.size());
    for(auto& i : (*it).second)
        res.push_back(i);
    return res;
}




// returns all the cells as output of a given cell
std::vector<const Cell *> Circuit::GetOutputsOfCell(int cell) const
{
    auto it = adjList.begin();
    for(int i = 0; i < cell; i++)
        it++;
    std::vector<const Cell*> res;
    res.reserve((*it).second.size());
    for(auto& i : (*it).second)
        res.push_back(i);
    return res;
}

//same as above but implemented wiht pointer search
std::vector<const Cell *> Circuit::GetOutputsOfCell(const Cell *cell) const
{
    auto it = std::find_if(adjList.begin(), adjList.end(),
                           [=](auto& c)
    {return &c.first == cell;});

    if(it == adjList.end())
        throw std::invalid_argument("no matchin cell in circuit");

    std::vector<const Cell*> res;
    res.reserve((*it).second.size());
    for(auto& i : (*it).second)
        res.push_back(i);
    return res;
}


//  follows the 4 types of described function ChangeCell: 2 based on
//  random replacing and 2 based on p-value replacing.
void Circuit::ChangeCell(const Cell *c) const
{
    if(c->type == "void")
        return;

    Cell* it = &std::find_if (adjList.begin(), adjList.end(),
                             [=](std::pair<Cell, std::vector<Cell*>>& cell)
    {return &cell.first == c;})->first;

    const auto it2 = std::find_if (cellSelection->begin(), cellSelection->end(),
                             [=](const std::vector<Cell>& cells)
    {return cells[0].type == it->type;});

    if(it2 == cellSelection->end())
        throw std::invalid_argument("no cell found in cell-selection");

    if(it2->size() != 0)
    {
        size_t rnd = rand()%it2->size();
        (*it).CopyParams(&(*it2)[rnd]);
    }
    else
    {
        throw std::logic_error("unknown error");
    }


}

bool Circuit::ChangeCell(const Cell *c, double p) const
{
    if(c->type == "void")
        return 0;

    auto it = &std::find_if (adjList.begin(), adjList.end(),
                             [=](std::pair<Cell, std::vector<Cell*>>& cell)
    {return &cell.first == c;})->first;

    auto it2 = std::find_if (cellSelection->begin(), cellSelection->end(),
                             [=](const std::vector<Cell>& cells)
    {return cells[0].type == it->type;});

    if(it2 == cellSelection->end())
        throw std::invalid_argument("no cell found in cell-selection");

    if(it2->size() != 0)
    {
        size_t cell = static_cast<size_t>(p*(it2->size()-1));

        if((*it) != (*it2)[cell])
        {
            (*it).CopyParams(&(*it2)[cell]);
            return true;
        }
        return false;

    }
    else
    {
        throw std::logic_error("unknown error");
    }
}

void Circuit::ChangeCell(size_t i) const
{
    if(inputLists[i].first->type == "void")
        return;

    auto it = inputLists[i].first;
    auto it2 = std::find_if (cellSelection->begin(), cellSelection->end(),
                             [=](const std::vector<Cell>& cells)
    {return cells[0].type == inputLists[i].first->type;});

    if(it2->size() != 0)
    {
        size_t cell = rand()%it2->size();

        if((*it) != (*it2)[cell])
            (*it).CopyParams(&(*it2)[cell]);

    }
    else
    {
        throw std::logic_error("unknown error");
    }
}

bool Circuit::ChangeCell(size_t i, double p) const
{
    if(inputLists[i].first->type == "void")
        return 0;

    auto it = inputLists[i].first;
    auto it2 = std::find_if (cellSelection->begin(), cellSelection->end(),
                             [=](const std::vector<Cell>& cells)
    {return cells[0].type == inputLists[i].first->type;});

    if(it2->size() != 0)
    {
        if(p > 1 || p < 0)
            throw std::invalid_argument("cell indicator must be between 0 and 1");

        size_t cell = static_cast<size_t>(p*(it2->size()-1));
        if((*it).name != (*it2)[cell].name)
        {
            (*it).CopyParams(&(*it2)[cell]);
            return true;
        }
        return false;

    }
    else
    {
        throw std::logic_error("unknown error");
    }

}

double Circuit::GetAreaOccupation() const
{
    double area = 0;
    //std::cout << "area occupation" << std::endl;
    std::for_each(adjList.begin(), adjList.end(),
                  [&](auto cellp)
    {return area += cellp.first.area;});

    return area;
}






