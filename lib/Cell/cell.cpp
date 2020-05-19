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



double Cell::GetTimingInfo(double intransit, double outCap, size_t output, size_t infoType) const
{
    if(this->type != "void")
        return timingInfo[output][infoType].GetDelay(intransit, outCap);

    return 0;
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
    int n = c.infoType.find(" ");
    c.infoType.erase(n, c.infoType.size()-n);

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

double CellTimingInfo::GetDelay(double inputTransition, double outCapacitance) const
{
    //finds the range of capacitange according to the input transition
    struct Comp
    {
        bool operator() ( const std::vector<double> v, double val ) const { return v[CellTimingInfo::inTransitIndex] < val; }
        bool operator() (double val,  const std::vector<double> v) const { return v[CellTimingInfo::inTransitIndex] > val; }
    };
    inputTransition = ApproxTransition(inputTransition);
    auto p = std::equal_range(values.begin(), values.end(), inputTransition, Comp{});


    //find the two iterators that describes the entries of the matrix among which should stay the out capacitance
    double prevCap = 0, succCap = 0;
    double prevT = 0, succT = 0;

    auto previt = std::max_element(p.first, p.second, [=](std::vector<double> v1, std::vector<double> v2)
    {
        bool res = v1[outCapIndex] < v2[outCapIndex];
        if(res && v2[outCapIndex] <= outCapacitance)
            return true;
        else
            return false;
    });

    auto succit = p.second;
    for(auto iter = --succit; iter != p.first-1; iter--)
    {
        if((*iter)[outCapIndex] < (*succit)[outCapIndex] && (*iter)[outCapIndex] >= outCapacitance)
            succit = iter;
    }

    if(succit == previt)
    {
        if((*succit)[outCapIndex] < outCapacitance)
        {
            if(succit != values.end()-1)
                succit++;
            else
                previt = values.end();
        }
        else
        {
            if (previt != values.begin())
                previt--;
            else
                succit = values.end();
        }
    }

    if(succit != values.end())
    {
        succCap = (*succit)[outCapIndex];
        succT = (*succit)[timeIndex];
    }
    if(previt != values.end())
    {
        prevCap = (*previt)[outCapIndex];
        prevT = (*previt)[timeIndex];
    }


    return (outCapacitance - succCap)/(prevCap-succCap)*prevT -
            (outCapacitance - prevCap)/(prevCap-succCap)*succT;
}

void CellTimingInfo::Test()
{
    std::vector<std::vector<double>> tester
    {
        {0.0, 0.0002},
        {0.005, 0.0150},
        {0.01, 0.2},
        {0.12, 0.050},
        {0.24, 0.00010},
        {0.24, 0.0098},
        {0.30, 0.02},
        {0.18, 0.0630},
        {0.50, 0.0180},
        {0.50, 0.50}
    };
    //double res = GetDelay(tester[0], tester.second);

    for(size_t i = 0; i < tester.size(); i++)
    {
        std::cout << GetDelay(tester[i][0], tester[i][1]) << std::endl;
    }
    return;
}



double CellTimingInfo::ApproxTransition(double transition) const
{
    // approx the input transition time to the values of this cell

    if(transition == 0)
        return values[0][inTransitIndex];

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

