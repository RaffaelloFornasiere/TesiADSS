#ifndef CELL_H
#define CELL_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

//  *****************************************************************************
//  Stores the data about pin names and capacity. Pin names are used to identify
//  the connections among the cells in the circuit.
//  all data is public in order to keep the code simple, furthermore
//  pins will be private objects of cells, so there is no need to provide the
//  information hiding paradigm
//  *****************************************************************************

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


//  *****************************************************************************
//  Represent a handler to read and sto the data about the different timing
//  parameters about the cell. these parameters in a matricial form where columns
//  represent in order:
//  input_net_transition total_out_capacitance delay_time
//  so in the matrix each row represent a set of this parameters that describe
//  almost completely the cell behavior on different input and output parameters.
//  This class store the entire matrix and provide a function that takes the
//  input_transition value and the out_capacitance and return the corresponding delay
//  value
//  *****************************************************************************

class CellTimingInfo
{
    friend std::istream& operator>>(std::istream& is, CellTimingInfo& c);
    friend std::ostream& operator<<(std::ostream& os, CellTimingInfo& c);

public:
    CellTimingInfo(std::string infoType = "") : infoType(infoType) {}


    double GetDelay (double inputTransition, double outCapacitance) const;

    // *********************SETTERS AND GETTERS**********************************
    void setInfoType(const std::string &value){infoType = value;}
    std::string getInfoType() const {return infoType;}
    // **************************************************************************


    // *******************TEST FUNCTION OF DELAY CALCULATION*********************
    void Test();
    // **************************************************************************

private:
    double ApproxTransition(double transition) const;

    std::string infoType;
    std::vector<std::vector<double>> values;

    const static int inTransitIndex = 0;
    const static int outCapIndex = 1;
    const static int timeIndex = 2;
};






//  *****************************************************************************
//  The class it's an input inferface and represent the whole cell structure,
//  has a name, a type (AND, OR, XOR, HA, etc...), a set of inputs and outputs
//  (stored as Pin objects) and for each output stores the timing data about
//  the different delays of the cell as CellTimingInfo objects.
//  *****************************************************************************

class Cell
{    
    friend class Circuit;

    friend std::istream& operator>>(std::istream& is, Cell& c);
    friend std::ostream& operator<<(std::ostream& os, const Cell& c);
    //bool operator=(const Cell& c2);

public:
    Cell(std::string cellName = "") : name(cellName){}

    // *********************SETTERS AND GETTERS**********************************
    std::string getName() const {return name;}
    void setName(const std::string &value){name = value;}

    std::string getType() const{return type;}
    void setType(const std::string &value){type = value;}
    // **************************************************************************



    // *******************GETTERS OF CELL PARAMETERS*****************************
    double GetInPinCapacity(int i) const {return input[i].capacity;}
    double GetOutPinCapacity(int i) const {return  output[i].capacity;}
    double GetTimingInfo(double intransit, double outCap, size_t output, size_t type) const;


    size_t GetNumOfInputs() const {return input.size();}
    size_t GetNumOfOutputs() const {return output.size();}

    std::string GetInputName(int i) const {return input[i].name;}
    std::string GetOutputName(int i) const {return output[i].name;}
    // **************************************************************************


    //
    void TestTimingInfo(){timingInfo[0][0].Test();}
    //


    // *********************CONST PUBLIC ATTRIBUTES******************************
    static const size_t cell_fall = 0;
    static const size_t cell_rise = 1;
    static const size_t fall_transition = 2;
    static const size_t rise_transition = 3;
    // **************************************************************************

private:
    std::string name;
    std::string type;

    double area;


    std::vector<Pin> input;
    std::vector<Pin> output;

    std::vector<std::vector<CellTimingInfo>> timingInfo;
};


#endif // CELL_H
