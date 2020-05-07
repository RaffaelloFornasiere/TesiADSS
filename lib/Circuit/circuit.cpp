#include "circuit.h"


std::ostream& operator<<(std::ostream &os, const Circuit &c)
{
    for(auto x : c.adjList)
    {
        os << "\"" << x.first.getName() << "(" << x.first.getType() << ")" << "\"";
        for(auto y : x.second)
        {
            if(y->getType() != "")
                os// <<"\"" << x.first.name << "(" << x.first.type << ")" << "\""
                        << "->" << "\""<< y->getName() << "(" << y->getType() << ")" << "\"";
            //<<std::endl;

        }
        os << std::endl;
    }
    return os;
}

Circuit::Circuit(std::vector<std::vector<Cell>> *selection)
    : cellSelection(selection)
{}


void Circuit::AssignRandom()
{

    for (auto& it : adjList)
    {
        for(size_t i = 0; i < cellSelection->size(); i++)
        {
            if(cellSelection->at(i)[0].type == it.first.type)
            {
                int j = rand()%cellSelection->at(i).size();
                it.first = cellSelection->at(i)[j];
            }
        }
    }

    return;

}

std::list<std::pair<Cell, std::vector<Cell*>>>::iterator Circuit::searchOutputSingal(std::string name)
{

    return std::find_if(adjList.begin(),
                        adjList.end(),
                        [=](std::pair<Cell, std::vector<Cell*>> p)
    {
        return std::find(p.first.output.begin(), p.first.output.end(), name) != p.first.output.end();
    });
}

// returns the iterator of the type searched
std::vector<std::vector<Cell>>::iterator Circuit::searchCellType(std::string type)
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


    if(i != adjList.end())
    {
        //aggiunge tale cella alla lista in input di questa cella e
        inputLists.back().second.push_back(&(*i).first);

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
    if(i != adjList.end())
    {
        //aggiunge tale cella alla lista in input di questa cella e
        inputLists.back().second.push_back(&(*i).first);

        //aggiunge alla
        (*i).second.push_back(&adjList.back().first);
    }

#ifdef CIRCUIT_DEBUG
    std::cout << "-" << line<<"-"<<std::endl;
#endif

    return true;
}

bool Circuit::readInstruction2(std::string line)
{
    // resize del vettore
    std::string name = "port" + std::to_string(adjList.size());
    adjList.emplace_back(std::make_pair(Cell(name), std::vector<Cell*>()));
    inputLists.emplace_back(std::make_pair(&adjList.back().first, std::vector<Cell*>()));

    if(name == "port76")
        std::cout << "";
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
    if(i != adjList.end())
    {
        //aggiunge tale cella alla lista in input di questa cella e
        inputLists.back().second.push_back(&(*i).first);

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



std::istream& operator>>(std::istream &is, Circuit &c)
{

    std::string aux;
    int g = is.tellg();

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

