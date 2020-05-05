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
    CellTimingInfo(std::string infoType = ""): infoType(infoType) {}
    double GetDelay(double inputTransition, double outCapacitance);


    void setInfoType(const std::string &value){infoType = value;}
    std::string getInfoType() const{return infoType;}

private:
    double ApproxTransition(double transition);

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
    friend class Circuit;

public:

    Cell(std::string cellName = "") : name(cellName){}


    std::string getName() const {return name;}
    void setName(const std::string &value){name = value;}

    std::string getType() const{return type;}
    void setType(const std::string &value){type = value;}

private:
    std::string name;
    //double area;
    //double leakage;
    std::vector<Pin> input;
    std::vector<Pin> output;
    //std::vector<std::string> function;
    std::string type;
    //std::vector<CellTimingInfo> timingInfo;
    std::vector<std::vector<CellTimingInfo>> timingInfo;
};


#endif // CELL_H
