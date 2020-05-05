#include "cell.h"

std::istream& operator>>(std::istream &is, Cell &c)
{
    std::string aux;
    int g = is.tellg();
    is >> c.name;
    //if(c.name == "FA1X18")
    //    std::cout << "aaaa";
    while(std::getline(is>>std::ws, aux) && aux.find("leakage") == std::string::npos);

    Pin p;
    while(is >> p && p.name != "")
    {
        if(p.dir == Pin::input)
        {
            c.input.emplace_back(p);
        }
        else
        {
            c.output.emplace_back(p);


            std::getline(is >> std::ws, aux);
            size_t i = 0;
            if((i = aux.find("---")) == std::string::npos)
            {
                //check if it is half adder or fulladder
                if(c.name.find("HA") == std::string::npos && c.name.find("FA") == std::string::npos)
                {
                    c.name = "";
                    is.seekg(g);
                    return is;
                }
                else
                {
                    if(c.name.find("HA") != std::string::npos)
                        c.type = "HA";
                    else
                        c.type = "FA";
                }
            }

            else
            {
                c.type = aux.substr(i+4, aux.size()-1);
                if(!isdigit(*(--c.type.end())))
                {
                    c.type += std::to_string(c.input.size());
                }
            }

            c.timingInfo.emplace_back();
            do
            {
                c.timingInfo.back().emplace_back();
            }while(is >> c.timingInfo.back().back() && c.timingInfo.back().back().getInfoType() != "");
            c.timingInfo.back().pop_back();
        }
    }


    return is;
}

std::ostream& operator<<(std::ostream &os, const Cell &c)
{
    os << c.name << "\n";
    os << "leakage" << "\n";
    for(auto x : c.input)
        os << x << "\n";


    for(size_t i = 0; i < c.output.size() && i < c.timingInfo.size(); i++)
    {
        auto x = c.output[i];
        os << x << "  (fff)";
        if(i == 0 && c.type != "HA" && c.type != "FA")
            os << " --- " << c.type;
        os  << "\n";
        for(size_t j = 0; j < 4 && j < c.timingInfo[i].size(); j++)
        {
            auto aux = c.timingInfo[i][j];
            os << aux;
        }

    }
    return os;
}



//-----------------------------------------------------------------------------------------------------------------------------
std::istream& operator>>(std::istream &is, CellTimingInfo& c)
{
    is.flags(std::ios_base::fmtflags(is.flags()|std::ios::binary));
    std::string aux;
    int g = is.tellg();

    std::smatch maches;
    std::regex pattern{R"***((([\w_]+ +)+\(\w+\)))***"};
    if(!std::getline(is>>std::ws, aux) || !std::regex_search(aux, maches, pattern))
    {
        is.seekg(g);
        c.infoType = "";
        return is;
    }
    c.infoType = *maches.begin();


    if(!std::getline(is>>std::ws, aux) &&
            aux.find("input_net_transition total_output_net_capacitance time") == std::string::npos)
    {
        is.seekg(g);
        c.infoType = "";
        return is;
    }

    //now the syntax is suppposed to be correct
    g = is.tellg();
    try {
        int j = 0;
        c.values.emplace_back(3);
        bool end = false;
        while(is >> aux && !end)
        {
            c.values.back()[j] = std::stod(aux);

            if(++j == 3)
            {
                c.values.emplace_back(3);
                j = 0;
            }
            g = is.tellg();
        }
    } catch (std::invalid_argument e) {
        //std::cerr << "errore in: " << e.what() << std::endl;
        //catches when there's no more a tuple of doubles. the error is thrown by std::stod
        c.values.pop_back();
        is.seekg(g);
    }

    return is;
}

std::ostream& operator<<(std::ostream &os, CellTimingInfo &c)
{
    os << c.infoType << "\n";
    os << "input_net_transition total_output_net_capacitance time" << "\n";
    for(auto x : c.values)
    {
        for(auto y : x)
            os << y << " ";
        os << "\n";
    }
    return os;
}

double CellTimingInfo::GetDelay(double inputTransition, double outCapacitance)
{
    //find the range of capacitange according to the input transition
    struct Comp
    {
        bool operator() ( const std::vector<double> v, double val ) const { return v[CellTimingInfo::inTransitIndex] < val; }
        bool operator() (double val,  const std::vector<double> v) const { return v[CellTimingInfo::inTransitIndex] > val; }
    };
    inputTransition = ApproxTransition(inputTransition);
    auto p = std::equal_range(values.begin(), values.end(), inputTransition, Comp{});



    auto i = std::find_if(p.first, p.second, [=](std::vector<double> v){return v[CellTimingInfo::outCapIndex] == outCapacitance;});
    if(i != p.second)
        return (*i)[CellTimingInfo::timeIndex];


    //find the two iterators that describes the entries of the matrix among which should stay the out capacitance
    std::vector<std::vector<double>>::iterator it = p.first;
    double prevCap = 0, succCap = 0;
    double prevT = 0, succT = 0;
    p.second--;
    while((*it)[outCapIndex] < outCapacitance && it != p.second) it++;
    if(it != values.begin())
    {
        if(it != p.second)
            it--;
        prevCap = (*it)[outCapIndex];
        prevT = (*it)[timeIndex];
    }
    it = p.second;
    while((*it)[outCapIndex] > outCapacitance && it != p.first) it--;

    if(it != --values.end())
    {
        if(it != p.first)
            it++;
        succCap = (*it)[outCapIndex];
        succT = (*it)[timeIndex];
    }

    return (outCapacitance - succCap)/(prevCap-succCap)*prevT -
            (outCapacitance - prevCap)/(prevCap-succCap)*succT;
}



double CellTimingInfo::ApproxTransition(double transition)
{
    // approx the input transition time to the values of of this cell

    double k = 1;
    double auxk = 1;
    double stdTr = 0;

    for(auto x : values)
    {
        auxk = std::abs((x[inTransitIndex] - transition)/(x[inTransitIndex] + transition));
        if(auxk < k)
        {
            k = auxk;
            stdTr = x[inTransitIndex];
        }
    }
    return stdTr;
}




//---------------------------------------------------------------------------------------------------------------





std::istream& operator>>(std::istream &is, Pin &p)
{
    is.flags(std::ios_base::fmtflags(is.flags()|std::ios::binary));
    std::string aux;
    int g = is.tellg();

    std::smatch maches;
    std::regex pattern{R"***((\w *((->)|(<-)) *\([-+]?\d*\.?\d*\)))***"};
    if(!std::getline(is>>std::ws, aux) || !std::regex_search(aux, maches, pattern))
    {
        is.seekg(g);
        p.name = "";
        return is;

    }
    std::stringstream(aux) >> std::ws >> p.name;

    if(aux.find('<') != std::string::npos)
        p.dir = Pin::input;
    else
        p.dir = Pin::output;

    p.capacity = std::stod(aux.substr(aux.find('(')+1, (g = aux.find(')'))));
    int d = (aux.size() - g);
    is.seekg(int(is.tellg()) - d);

    return is;
}

std::ostream& operator<<(std::ostream &os, Pin &p)
{
    os << p.name;
    if(p.dir == Pin::input)
        os << " " << "<-";
    else
        os << " " << "->";
    os << " (" << p.capacity << ")";

    return os;
}









