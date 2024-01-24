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
    double total_power , total_toggle_times , coverage;
    
    vector<Gate *> sorted_delay_gates;
    vector<Gate *> sorted_load_gates;
    vector<Gate *> input_gates;
    vector<Gate *> output_gates;
    vector<Gate *> long_path_gates;
    vector<Gate *> smallest_path_gates;
    vector<Gate *> new_smallest_path_gates;
    vector<double> output_load_vec;
    vector<double> in_tran_time_vec;
    NetList(const string);
    ~NetList();
    const vector<Wire *> &getInputs();
    const vector<Wire *> &getOutputs();
    const unordered_map<string, Wire *> &getWires();
    const vector<Gate *> &getGates();
    void print() const;
    void find_Output_load_path() const;
    void clear_ouput_load();
    Wire* set_Output_value(Gate *);
    void set_Output_load() const;
    void find_index_1();
    void set_input_index_2();
    void Table_Interpolation();
    void check_input_tran_time(Gate *);
    bool check_cell_delay();
    void clear_wire_transition();
    void sort_delay();
    void find_path();
    void find_longest_path(Gate *);
    void find_smallest_path(Gate *);
    void set_input_pattern(const vector<bool> &, const unordered_map<string, int> &);
    void set_initial_gate_value();
    void set_value(Wire *);
    void forward(const vector<bool> &, const unordered_map<string, int> &);
    void power_Table_Interpolation();
    void clear_parameters();
    void calculate_toggle();
};