#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
using namespace std;
class Table
{
public:
    string type;
    vector<vector<double>> values;
    Table(ifstream &, string &);
    void print(const int = 0);
};
class Internal_power
{
public:
    Table *rise_power, *fall_power;
    Internal_power(ifstream &);
    ~Internal_power();
    void print(const int = 0);
};
class Timing
{
public:
    Table *cell_rise, *cell_fall, *rise_transition, *fall_transition;
    Timing(ifstream &);
    ~Timing();
    void print(const int = 0);
};
class Pin
{
public:
    string direction;
    double capacitance;
    Internal_power *internal_power;
    Timing *timing;
    Pin(ifstream &);
    ~Pin();
    void print(const int = 0);
};
class Cell
{
public:
    unordered_map<string, Pin *> pins;
    Cell(ifstream &);
    ~Cell();
    void print(const int = 0);
};
class Liberty
{
public:
    unordered_map<string, Cell *> cells;
    Liberty(const string);
    ~Liberty();
    void print();
};