#include <string>
#include <iostream>
#include "Gate.h"
#include "Wire.h"
#include <cmath>
using namespace std;
Wire::Wire(const string n)
{
    input = nullptr;
    outputs.clear();
    name = n;
}
void Wire::setInput(Gate *gate)
{
    input = gate;
}
void Wire::setOutput(Gate *gate)
{
    outputs.emplace_back(gate);
}
string Wire::getName() const
{
    return name;
}
vector<Gate *> Wire::getOutput()
{
    return outputs;
}
Gate * Wire::getInput()
{
    return input;
}
void Wire::print() const
{
    cout << "Wire {" << endl;
    cout << "\tName: " << name << endl;
    cout << "\tInput: " << (input ? input->getName() : "none") << endl;
    cout << "\tOutputs:";
    for (const Gate *output : outputs)
    {
        cout << " " << output->getName();
    }
    cout << endl;
    cout << "}" << endl;
}

bool Wire::isEqual(double a , double b)
{
    return std::abs(a - b) < epsilon;
}

bool Wire::isGreaterThan(double a , double b)
{
    return (a - b) > epsilon;
}

bool Wire::isLessThan(double a , double b)
{
    return (a - b) < epsilon;
}