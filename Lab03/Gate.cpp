#include <iostream>
#include <vector>
#include <string>
#include "Wire.h"
#include "Gate.h"
#include <cmath>
using namespace std;
Gate::Gate(const vector<string> &words, unordered_map<string, Wire *> &wires)
{
    output_load = 0.0000000;
    input_tran_time_A1 = 0.0000000;
    input_tran_time_A2 = 0.0000000;
    output_tran_time = 0.0000000;
    input_tran_time = 0.0000000;
    propagation_delay = 0.0000000;
    real_switching_power = 0.0000000;

    output_value = -1;
    index_1_1 = -1;
    index_1_2 = -1;
    index_2_1 = -1;
    index_2_2 = -1;
    ready = 0;
    done = 0;
    pre_pattern_output = 0;
    toggle_0to1 = 0;
    toggle_1to0 = 0;

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
