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
    double output_load;
    double input_tran_time_A1, input_tran_time_A2, input_tran_time;
    int output_value;
    int index_1_1, index_1_2;
    int index_2_1, index_2_2;
    int ready, done;
    double output_tran_time;
    double propagation_delay;
    double total_propagation_delay;
    Wire *input_wire;

    vector<Gate *> propagation_gate;
    vector<double> output_load_vec = {0.001400, 0.003000, 0.006200, 0.012500, 0.025100, 0.050400, 0.101000};
    vector<double> in_tran_time_vec = {0.0208, 0.0336, 0.06, 0.1112, 0.2136, 0.4192, 0.8304};
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