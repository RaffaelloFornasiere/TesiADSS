#ifndef CELL_H
#define CELL_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

class Pin
{
    friend std::istream& operator>>(std::istream& is, Pin& p);
    friend std::ostream& operator<<(std::ostream& is, Pin& p);

    friend bool operator==(const Pin& p, const std::string s){return p.name == s;}
    friend bool operator==(const std::string s, const Pin& p){return p.name == s;}

public:
    Pin(std::string name = "") : name(name) {}
    std::string name;
    bool dir;
    double capacity;
    const static int input  = 0;
    const static int output = 1;

};



class CellTimingInfo
{
    friend std::istream& operator>>(std::istream& is, CellTimingInfo& c);
    friend std::ostream& operator<<(std::ostream& os, CellTimingInfo& c);

public:
    CellTimingInfo(std::string infoType = "") : infoType(infoType) {}
    double GetDelay (double inputTransition, double outCapacitance) const;


    void setInfoType(const std::string &value){infoType = value;}
    std::string getInfoType() const {return infoType;}

    void Test();

private:
    double ApproxTransition(double transition) const;

    std::string infoType;
    std::vector<std::vector<double>> values;

    const static int inTransitIndex = 0;
    const static int outCapIndex = 1;
    const static int timeIndex = 2;
};



class Cell
{
    friend std::istream& operator>>(std::istream& is, Cell& c);
    friend std::ostream& operator<<(std::ostream& os, const Cell& c);
    //bool operator=(const Cell& c2);

    friend class Circuit;

public:

    Cell(std::string cellName = "") : name(cellName){}


    std::string getName() const {return name;}
    void setName(const std::string &value){name = value;}

    std::string getType() const{return type;}
    void setType(const std::string &value){type = value;}

    double GetInPinCapacity(int i) const {return input[i].capacity;}
    double GetOutPinCapacity(int i) const {return  output[i].capacity;}

    size_t GetNumOfInputs() const {return input.size();}
    size_t GetNumOfOutputs() const {return output.size();}

    std::string GetInputName(int i) const {return input[i].name;}
    std::string GetOutputName(int i) const {return output[i].name;}


    double GetTimingInfo(double intransit, double outCap, size_t output, size_t type) const;
    static const size_t cell_fall = 0;
    static const size_t cell_rise = 1;
    static const size_t fall_transition = 2;
    static const size_t rise_transition = 3;

//    std::vector<std::vector<CellTimingInfo>> timingInfo;

private:
    std::string name;
    //double area;
    //double leakage;

    std::vector<Pin> input;
    std::vector<Pin> output;

    std::string type;
    std::vector<std::vector<CellTimingInfo>> timingInfo;
};


#endif // CELL_H
