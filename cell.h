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
public:
    std::string name;
    bool dir;
    double capacity;
    const static int input  = 0;
    const static int output = 1;

};


class CellTimingInfo
{
    friend std::istream& operator>>(std::istream& is, CellTimingInfo& c);
public:
    CellTimingInfo(std::string infoType = ""): infoType(infoType) {}
    double GetPropagationDelay(double inputTransition, double outCapacitance);

    std::string getInfoType() const;
    void setInfoType(const std::string &value);

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
public:
    Cell();


    std::string getName() const;

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
