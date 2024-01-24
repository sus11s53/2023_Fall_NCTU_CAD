#include <string>
#include <vector>
#include "Gate.h"
#include "Wire.h"
using namespace std;
class NetList
{
private:
    vector<Wire *> inputs;
    vector<Wire *> outputs;
    unordered_map<string, Wire *> wires;
    vector<Gate *> gates;

public:
    vector<Gate *> sorted_delay_gates;
    vector<Gate *> sorted_propagation_delay_gates;
    vector<Gate *> sorted_load_gates;
    vector<Gate *> output_gates;
    vector<Gate *> long_path_gates;
    vector<Gate *> smallest_path_gates;
    vector<Wire *> long_path_gates_wire;
    vector<Wire *> smallest_path_gates_wire;
    double longest_path_delay;
    double smallest_path_delay;
    NetList(const string);
    ~NetList();
    const vector<Wire *> &getInputs();
    const vector<Wire *> &getOutputs();
    const unordered_map<string, Wire *> &getWires();
    const vector<Gate *> &getGates();
    void print() const;
    void find_Output_load_path() const;
    void clear_ouput_load();
    void set_Output_value() const;
    void set_Output_load() const;
    void find_index_1();
    void set_input_index_2();
    double Table_Interpolation();
    void check_input_tran_time(Gate *);
    bool check_cell_delay();
    void clear_wire_transition();
    void sort_delay();
    void find_path();
    void find_longest_path(Gate *);
    void find_smallest_path(Gate *);
};