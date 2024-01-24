#include <vector>
#include <string>
using namespace std;
class Gate;
class Wire
{
private:
    Gate *input;
    vector<Gate *> outputs;

    string name;

public:
    int wire_type;
    double transition_delay;
    double pre_gate_propagation_delay;
    double total_propagation_delay;
    int wire_ready;
    Wire()
    {
        wire_type = -1;
        transition_delay = 0.0000000;
        wire_ready = 0;
        pre_gate_propagation_delay = 0.0000000;
    }
    Wire(const string);
    void setInput(Gate *);
    void setOutput(Gate *);
    vector<Gate *> getOutput();
    string getName() const;
    Gate * getInput();
    void print() const;
    double epsilon = 1e-10;
    bool isEqual(double,double);
    bool isGreaterThan(double,double);
    bool isLessThan(double,double);
};