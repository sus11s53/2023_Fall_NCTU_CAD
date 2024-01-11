#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
class Wire;
class Gate
{
private:
    unordered_map<string, Wire *> inputs;
    Wire *output;

    string type;
    string name;

public:
    bool value;
    int prerequisite;
    
    double output_load;
    double input_tran_time_A1, input_tran_time_A2, input_tran_time;
    int output_value;
    int index_1_1, index_1_2;
    int index_2_1, index_2_2;
    int ready, done;
    double output_tran_time;
    double propagation_delay;
    Wire *input_wire;
    double internal_power;
    double switching_power;
    double real_switching_power;
    int pre_pattern_output;
    int toggle_0to1 , toggle_1to0;

    vector<int> input_value;
    Gate(const vector<string> &, unordered_map<string, Wire *> &);
    void setInput(const string, Wire *);
    void setOutput(Wire *);
    unordered_map<string, Wire *> getInputs();
    Wire *getOutput() const;
    string getType();
    string getName() const;
    unordered_map<string, vector<string>> output_load_type;
    void print() const;
    double epsilon = 1e-10;
    bool isEqual(double, double);
    bool isGreaterThan(double, double);
    bool isLessThan(double, double);
};