#include <iostream>
#include <vector>
#include <string>
#include "Wire.h"
#include "Gate.h"
#include <cmath>
using namespace std;
Gate::Gate(const vector<string> &words, unordered_map<string, Wire *> &wires)
{

    output_load_vec = {0.001400, 0.003000, 0.006200, 0.012500, 0.025100, 0.050400, 0.101000};
    in_tran_time_vec = {0.0208, 0.0336, 0.06, 0.1112, 0.2136, 0.4192, 0.8304};
    output_load = 0.0000000;
    input_tran_time_A1 = 0.0000000;
    input_tran_time_A2 = 0.0000000;
    output_tran_time = 0.0000000;
    input_tran_time = 0.0000000;
    propagation_delay = 0.0000000;
    total_propagation_delay = 0.0000000;

    output_value = -1;
    index_1_1 = -1;
    index_1_2 = -1;
    index_2_1 = -1;
    index_2_2 = -1;
    ready = 0;
    done = 0;

    inputs.clear();
    output = nullptr;
    type = words[0];
    name = words[1];
    for (int i = 2; i < words.size(); i = i + 2)
    {
        Wire *wire = wires[words[i + 1]];
        if (wire == nullptr)
        {
            cout << "Undeclared wire \"" << words[i + 1] << "\" found" << endl;
            exit(-1);
        }
        if (words[i] == "ZN")
        {
            output = wire;
            wire->setInput(this);
        }
        else
        {
            inputs[words[i]] = wire;
            wire->setOutput(this);
        }
    }
}

void Gate::setInput(const string pin, Wire *wire)
{
    inputs[pin] = wire;
}
void Gate::setOutput(Wire *wire)
{
    output = wire;
}
unordered_map<string, Wire *> Gate::getInputs()
{
    return inputs;
}
string Gate::getName() const
{
    return name;
}
Wire *Gate::getOutput() const
{
    return output;
}
string Gate::getType()
{
    return type;
}
void Gate::print() const
{
    cout << "Gate {" << endl;
    cout << "\tName: " << name << endl;
    cout << "\tType: " << type << endl;
    cout << "\tInputs:";
    for (unordered_map<std::string, Wire *>::const_iterator it = inputs.begin(); it != inputs.end(); ++it)
    {
        cout << " " << it->first << "(" << it->second->getName() << ")";
    }
    cout << endl;
    cout << "\tOutput: " << output->getName() << endl;
    cout << "}" << endl;
}

bool Gate::isEqual(double a, double b)
{
    return std::abs(a - b) < epsilon;
}

bool Gate::isGreaterThan(double a, double b)
{
    return (a - b) > epsilon;
}

bool Gate::isLessThan(double a, double b)
{
    return (a - b) < epsilon;
}
