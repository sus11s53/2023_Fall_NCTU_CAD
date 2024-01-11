#include <iostream>

#include "Liberty.h"

using namespace std;

Table::Table(ifstream &libertyFile, string &firstLine)
{
    const size_t typeBegin = firstLine.find("(") + 1, typeEnd = firstLine.find(")");
    type = firstLine.substr(typeBegin, typeEnd - typeBegin);
    values.clear();
    string line;
    while (getline(libertyFile, line))
        if (line.find("\"") != string::npos)
        {
            vector<double> row{};
            const size_t first = line.find("\"") + 1;
            const size_t second = line.find("\"", first);
            string data = line.substr(first, second - first);
            data.push_back(',');
            for (int begin = 0, end = 0; end < data.length(); ++end)
                if (data[end] == ',')
                {
                    row.emplace_back(stod(data.substr(begin, end - begin)));
                    begin = end + 1;
                }
            values.emplace_back(row);
        }
        else if (line.find("}") != string::npos)
            break;
}
void Table::print(const int depth)
{
    const string indentation(depth, '\t');
    for (const vector<double> &row : values)
    {
        cout << indentation;
        for (const double value : row)
            cout << value << " ";
        cout << endl;
    }
}

Internal_power::Internal_power(ifstream &libertyFile)
{
    rise_power = nullptr;
    fall_power = nullptr;
    string line;
    while (getline(libertyFile, line))
        if (line.find("rise_power") != string::npos)
            rise_power = new Table(libertyFile, line);
        else if (line.find("fall_power") != string::npos)
            fall_power = new Table(libertyFile, line);
        else if (line.find("}") != string::npos)
            break;
}
Internal_power::~Internal_power()
{
    delete rise_power;
    delete fall_power;
}
void Internal_power::print(const int depth)
{
    const string indentation(depth, '\t');
    cout << indentation << "rise_power(" << rise_power->type << ") {" << endl;
    rise_power->print(depth + 1);
    cout << indentation << "}" << endl;
    cout << indentation << "fall_power(" << fall_power->type << ") {" << endl;
    fall_power->print(depth + 1);
    cout << indentation << "}" << endl;
}

Timing::Timing(ifstream &libertyFile)
{
    cell_rise = nullptr;
    cell_fall = nullptr;
    rise_transition = nullptr;
    fall_transition = nullptr;
    string line;
    while (getline(libertyFile, line))
        if (line.find("cell_rise") != string::npos)
            cell_rise = new Table(libertyFile, line);
        else if (line.find("cell_fall") != string::npos)
            cell_fall = new Table(libertyFile, line);
        else if (line.find("rise_transition") != string::npos)
            rise_transition = new Table(libertyFile, line);
        else if (line.find("fall_transition") != string::npos)
            fall_transition = new Table(libertyFile, line);
        else if (line.find("}") != string::npos)
            break;
}
Timing::~Timing()
{
    delete cell_rise;
    delete cell_fall;
    delete rise_transition;
    delete fall_transition;
}
void Timing::print(const int depth)
{
    const string indentation(depth, '\t');
    cout << indentation << "cell_rise(" << cell_rise->type << ") {" << endl;
    cell_rise->print(depth + 1);
    cout << indentation << "}" << endl;
    cout << indentation << "cell_fall(" << cell_fall->type << ") {" << endl;
    cell_fall->print(depth + 1);
    cout << indentation << "}" << endl;
    cout << indentation << "rise_transition(" << rise_transition->type << ") {" << endl;
    rise_transition->print(depth + 1);
    cout << indentation << "}" << endl;
    cout << indentation << "fall_transition(" << fall_transition->type << ") {" << endl;
    fall_transition->print(depth + 1);
    cout << indentation << "}" << endl;
}

Pin::Pin(ifstream &libertyFile)
{
    internal_power = nullptr;
    timing = nullptr;
    string line;
    while (getline(libertyFile, line))
        if (line.find("direction") != string::npos)
        {
            const size_t begin = line.find(":") + 2, end = line.find(";");
            direction = line.substr(begin, end - begin);
        }
        else if (line.find("capacitance") != string::npos)
        {
            const size_t begin = line.find(":") + 2, end = line.find(";");
            capacitance = stod(line.substr(begin, end - begin));
        }
        else if (line.find("internal_power") != string::npos)
            internal_power = new Internal_power(libertyFile);
        else if (line.find("timing") != string::npos)
            timing = new Timing(libertyFile);
        else if (line.find("}") != string::npos)
            break;
}
Pin::~Pin()
{
    delete internal_power;
    delete timing;
}
void Pin::print(const int depth)
{
    const string indentation(depth, '\t');
    cout << indentation << "direction: " << direction << endl;
    cout << indentation << "capacitance: " << capacitance << endl;
    if (internal_power)
    {
        cout << indentation << "internal_power() {" << endl;
        internal_power->print(depth + 1);
        cout << indentation << "}" << endl;
    }
    if (timing)
    {
        cout << indentation << "timing() {" << endl;
        timing->print(depth + 1);
        cout << indentation << "}" << endl;
    }
}

Cell::Cell(ifstream &libertyFile)
{
    pins.clear();
    string line;
    while (getline(libertyFile, line))
        if (line.find("pin") != string::npos)
        {
            const size_t begin = line.find("(") + 1, end = line.find(")");
            pins[line.substr(begin, end - begin)] = new Pin(libertyFile);
        }
        else if (line.find("}") != string::npos)
            break;
}
Cell::~Cell()
{
    for (unordered_map<string, Pin *>::const_iterator it = pins.begin(); it != pins.end(); ++it)
        delete it->second;
}
void Cell::print(const int depth)
{
    const string indentation(depth, '\t');
    for (unordered_map<string, Pin *>::const_iterator it = pins.begin(); it != pins.end(); ++it)
    {
        cout << indentation << "pin(" << it->first << ") {" << endl;
        it->second->print(depth + 1);
        cout << indentation << "}" << endl;
    }
}

Liberty::Liberty(const string filePath)
{
    cells.clear();
    ifstream libertyFile(filePath);
    string line;
    while (getline(libertyFile, line))
        if (line.find("cell") != string::npos)
        {
            const size_t begin = line.find("(") + 1, end = line.find(")");
            cells[line.substr(begin, end - begin)] = new Cell(libertyFile);
        }
}
Liberty::~Liberty()
{
    for (unordered_map<string, Cell *>::const_iterator it = cells.begin(); it != cells.end(); ++it)
        delete it->second;
}
void Liberty::print()
{
    for (unordered_map<string, Cell *>::const_iterator it = cells.begin(); it != cells.end(); ++it)
    {
        cout << "cell(" << it->first << ") {" << endl;
        it->second->print(1);
        cout << "}" << endl;
    }
}
