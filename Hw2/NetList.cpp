#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "NetList.h"
#include "Liberty.h"
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;
NetList::NetList(const string fileName)
{
    inputs.clear();
    outputs.clear();
    wires.clear();
    gates.clear();
    ifstream fs(fileName);
    string line{};
    bool multiLineComment = false;
    while (getline(fs, line))
    {
        string temp{};
        if (!line.empty() && line[0] != '/' && line[0] != '*')
            temp.push_back(line[0]);
        for (int i = 1; i < line.length(); ++i)
        {
            if (line[i] == '/')
            {
                if (line[i - 1] == '/') // "//": start single line comment
                    if (multiLineComment)
                        continue;
                    else
                        break;
                else if (line[i - 1] == '*') // "*/": end multi line comment
                    if (multiLineComment)
                    {
                        multiLineComment = false;
                        temp.push_back(' ');
                    }
                    else
                    {
                        cout << "input file format error" << endl;
                        exit(-1);
                    }
            }
            else if (line[i] == '*')
            {
                if (line[i - 1] == '/') // "/*": start multi line comment
                    multiLineComment = true;
            }
            else if (!multiLineComment)
                temp.push_back(line[i]);
        }
        line.swap(temp);
        vector<string> words{};
        for (int begin = 0, end = 0; end < line.length(); ++end)
            if (line[end] == '\t' || line[end] == ' ' || line[end] == ',' || line[end] == ';' || line[end] == '.' || line[end] == '(' || line[end] == ')')
            {
                if (begin < end)
                    words.emplace_back(line.substr(begin, end - begin));
                begin = end + 1;
            }
        if (words.empty() || words[0] == "module" || words[0] == "endmodule")
            continue;
        if (words[0] == "input")
            for (int i = 1; i < words.size(); ++i)
            {
                Wire *wire = new Wire(words[i]);
                wires[words[i]] = wire;
                inputs.emplace_back(wire);
            }
        else if (words[0] == "output")
            for (int i = 1; i < words.size(); ++i)
            {
                Wire *wire = new Wire(words[i]);
                wires[words[i]] = wire;
                outputs.emplace_back(wire);
            }
        else if (words[0] == "wire")
            for (int i = 1; i < words.size(); ++i)
            {
                Wire *wire = new Wire(words[i]);
                wires[words[i]] = wire;
            }
        else
        {
            Gate *gate = new Gate(words, wires);
            gates.emplace_back(gate);
        }
    }
    fs.close();
}

NetList::~NetList()
{
    for (unordered_map<string, Wire *>::iterator it = wires.begin(); it != wires.end(); ++it)
        delete it->second;
    for (Gate *gate : gates)
        delete gate;
}
const vector<Wire *> &NetList::getInputs()
{
    return inputs;
}
const vector<Wire *> &NetList::getOutputs()
{
    return outputs;
}
const unordered_map<string, Wire *> &NetList::getWires()
{
    return wires;
}
const vector<Gate *> &NetList::getGates()
{
    return gates;
}
void NetList::print() const
{
    cout << "NetList {" << endl;
    cout << "\tInputs:";
    for (const Wire *input : inputs)
        cout << " " << input->getName();
    cout << endl;
    cout << "\tOutputs:";
    for (const Wire *output : outputs)
        cout << " " << output->getName();
    cout << endl;
    cout << "\tWires:";
    for (unordered_map<string, Wire *>::const_iterator it = wires.begin(); it != wires.end(); ++it)
        cout << " " << it->first;
    cout << endl;
    cout << "\tGates:";
    for (const Gate *gate : gates)
        cout << " " << gate->getName();
    cout << endl;
    cout << "}" << endl;
    for (unordered_map<string, Wire *>::const_iterator it = wires.begin(); it != wires.end(); ++it)
        it->second->print();
    for (const Gate *gate : gates)
        gate->print();
}

void NetList::find_Output_load_path() const
{

    
    for (Wire *outputWire : outputs)
        outputWire->getInput()->output_load_type["output"].push_back("outpin");
    for (Gate *gate : gates)
    {
        const unordered_map<string, Wire *> &gateInputs = gate->getInputs();
        for (unordered_map<string, Wire *>::const_iterator it = gateInputs.begin(); it != gateInputs.end(); ++it)
        {
            Gate *prevGate = it->second->getInput();
            if (prevGate == nullptr)
                continue;
            prevGate->output_load_type[gate->getType()].push_back(it->first);
        }
    } 

    //for (Gate *gate : gates)
    //{
    //    // 所有output wire的前一個gate要做output_load_type["output"].push_back("outpin");
    //    Wire *g1 = gate->getOutput();
    //    vector<Gate *> g2 = g1->getOutput();
    //
    //    for (vector<Wire *>::const_iterator it = outputs.begin(); it != outputs.end(); ++it)
    //    {
    //        if ((*it) == g1)
    //        {
    //            gate->output_load_type["output"].push_back("outpin");
    //        }
    //    }
    //    // 反過來做就不用在inputs中尋找了
    //    if (g2.size() != 0)
    //    {
    //        for (Gate *wire_output : g2)
    //        {
    //            unordered_map<string, Wire *> gate_input = wire_output->getInputs();
    //            for (unordered_map<string, Wire *>::const_iterator it = gate_input.begin(); it != gate_input.end(); ++it)
    //            {
    //                if (it->second == g1)
    //                {
    //                    gate->output_load_type[wire_output->getType()].push_back(it->first);
    //                }
    //            }
    //        }
    //    }
    //}

    // for (Gate *gate : gates)
    //{
    //     cout << gate->getName() << endl;
    //     cout << " ----------------------------------- " << endl;
    //     for(unordered_map<string,vector<string>>::const_iterator it = gate->output_load_type.begin(); it != gate->output_load_type.end(); ++it)
    //     {
    //         for(int i = 0; i < it->second.size(); i++)
    //         {
    //             cout << it->first << " , " << it->second[i] << endl;
    //         }
    //     }
    //     cout << " ----------------------------------- " << endl;
    // }
}

void NetList::clear_ouput_load()
{
    for (Gate *gate : gates)
    {
        gate->output_load = 0.0000000;
    }
}

void NetList::set_Output_load() const
{
    for (Gate *gate : gates)
    {
        for (unordered_map<string, vector<string>>::const_iterator it = gate->output_load_type.begin(); it != gate->output_load_type.end(); ++it)
        {
            if (it->first == "output")
            {
                gate->output_load = gate->output_load + 0.03;
            }
            else if (it->first == "INVX1")
            {
                for (int i = 0; i < it->second.size(); i++)
                {
                    gate->output_load = gate->output_load + 0.0109115;
                }
            }
            else if (it->first == "NOR2X1")
            {
                for (int i = 0; i < it->second.size(); i++)
                {
                    if (it->second[i] == "A1")
                        gate->output_load = gate->output_load + 0.0105008;
                    else if (it->second[i] == "A2")
                        gate->output_load = gate->output_load + 0.0108106;
                }
            }
            else if (it->first == "NANDX1")
            {
                for (int i = 0; i < it->second.size(); i++)
                {
                    if (it->second[i] == "A1")
                        gate->output_load = gate->output_load + 0.00683597;
                    else if (it->second[i] == "A2")
                        gate->output_load = gate->output_load + 0.00798456;
                }
            }
        }
    }

    // for (Gate *gate : gates)
    //{
    //     cout << gate->getName() << endl;
    //     cout << " ----------------------------------- " << endl;
    //     cout << fixed << setprecision(6) << gate->output_load << endl;
    //     cout << " ----------------------------------- " << endl;
    // }
}

void NetList::set_Output_value() const
{
    for (Gate *gate : gates)
    {
        if (gate->getType() == "INVX1")
        {
            gate->output_value = 0;
        }
        else
        {
            gate->output_value = 1;
        }
    }
}

void NetList::find_index_1()
{
    for (Gate *gate : gates)
    {
        for (int j = 0; j < gate->output_load_vec.size(); j++)
        {
            if (!gate->isGreaterThan(gate->output_load, gate->output_load_vec[j]))
            {
                if (gate->isEqual(gate->output_load, gate->output_load_vec[j]))
                {
                    gate->index_1_1 = j;
                    break;
                }
                else
                {
                    if (j != 0)
                    {
                        gate->index_1_1 = j - 1;
                        gate->index_1_2 = j;
                        break;
                    }
                    else
                    {
                        gate->index_1_1 = 0;
                        gate->index_1_2 = 1;
                        break;
                    }
                }
            }
            if (j == 6)
            {
                gate->index_1_1 = 5;
                gate->index_1_2 = 6;
                break;
            }
        }
    }

    // for (Gate *gate : gates)
    //{
    //     cout << gate->getName() << endl;
    //     cout << " ----------------------------------- " << endl;
    //     cout << gate->index_1_1 << " , " << gate->index_1_2 << endl;
    //     cout << " ----------------------------------- " << endl;
    // }
}

void NetList::clear_wire_transition()
{
    for (unordered_map<string, Wire *>::const_iterator it = wires.begin(); it != wires.end(); ++it)
    {
        it->second->transition_delay = 0.0;
        it->second->pre_gate_propagation_delay = 0.0000000;
        it->second->total_propagation_delay = 0.0000000;
    }
}

void NetList::set_input_index_2()
{
    for (Gate *gate : gates)
    {
        if (gate->ready != 1)
        {
            unordered_map<string, Wire *> gate_input = gate->getInputs();
            for (unordered_map<string, Wire *>::const_iterator it = gate_input.begin(); it != gate_input.end(); ++it)
            {
                if (it->second->getInput() == nullptr && gate->getType() == "INVX1")
                {

                    gate->input_tran_time_A1 = 0.00000000;
                    gate->input_tran_time_A2 = 0.00000000;
                    gate->ready = 1;
                    gate->index_2_1 = 0;
                    gate->index_2_2 = 1;
                    gate->input_tran_time = 0.00000000;
                    gate->input_wire = it->second;
                    it->second->transition_delay = 0.00000000;
                    // cout << gate->getName() << " is Input Inverter " << endl;
                    break;
                }
                else if (it->second->getInput() == nullptr)
                {
                    if (it->first == "A1")
                    {
                        gate->input_tran_time_A1 = 0.00000000;
                        it->second->transition_delay = 0.00000000;
                        // cout << gate->getName() << " , " << it->second->getName() << " , " << it->first << " , " << gate->input_tran_time_A2 << endl;
                        continue;
                    }
                    else if (it->first == "A2")
                    {
                        gate->input_tran_time_A2 = 0.00000000;
                        it->second->transition_delay = 0.00000000;
                        // cout << gate->getName() << " , " << it->second->getName() << " , " << it->first << " , " << gate->input_tran_time_A2 << endl;
                        continue;
                    }
                }
                else if (it->second->wire_ready == 1)
                {
                    if (it->first == "A1")
                    {
                        gate->input_tran_time_A1 = it->second->transition_delay;
                        // cout << gate->getName() << " , " << it->first << " , " << gate->input_tran_time_A1 << endl;
                        continue;
                    }
                    else if (it->first == "A2")
                    {
                        gate->input_tran_time_A2 = it->second->transition_delay;
                        // cout << gate->getName() << " , " << it->first << " , " << gate->input_tran_time_A2 << endl;
                        continue;
                    }
                    else if (gate->getType() == "INVX1")
                    {
                        gate->input_tran_time_A1 = it->second->transition_delay;
                        gate->input_tran_time_A2 = it->second->transition_delay;
                        gate->input_tran_time = gate->input_tran_time_A1;
                        gate->ready = 1;
                        gate->input_wire = it->second;
                        // cout << gate->getName() << " , " << it->first << " , " << gate->input_tran_time_A1 << endl;
                        break;
                    }
                }
            }
        }

        if (gate->done != 1)
        {
            check_input_tran_time(gate);
        }

        if (gate->ready == 1 && gate->done != 1)
        {
            // cout << gate->getName() <<" is ready!"<< " input_transition_time = " << gate->input_tran_time << " type is " << gate->getType() << endl;
            unordered_map<string, Wire *> gate_input = gate->getInputs();
            // for(unordered_map<string, Wire *>::iterator it = gate_input.begin(); it!= gate_input.end();++it)
            //{
            //     if(it->second->getInput() != nullptr)
            //     {
            //         cout << " Pre_gate is " << it->second->getInput()->getName() << " type is " << it->second->getInput()->getType() << " Pin is " << it->first << endl;
            //         cout << " Pre_wire is " << it->second->getName() << " Propagation_delay = " << it->second->total_propagation_delay << endl;
            //     }
            //     else
            //     {
            //         cout << " Input wire is " << it->second->getName() << " Propagation_delay = " << it->second->total_propagation_delay << " Pin is " << it->first << endl;
            //     }
            // }
            // cout << "--------------------------------------------" << endl;
            for (int j = 0; j < gate->in_tran_time_vec.size(); j++)
            {
                if (!gate->isGreaterThan(gate->input_tran_time, gate->in_tran_time_vec[j]))
                {
                    if (gate->isEqual(gate->input_tran_time, gate->in_tran_time_vec[j]))
                    {
                        gate->index_2_1 = j;
                        break;
                    }
                    else
                    {
                        if (j != 0)
                        {
                            gate->index_2_1 = j - 1;
                            gate->index_2_2 = j;
                            break;
                        }
                        else
                        {
                            gate->index_2_1 = 0;
                            gate->index_2_2 = 1;
                            break;
                        }
                    }
                }
                if (j == 6)
                {
                    gate->index_2_1 = 5;
                    gate->index_2_2 = 6;
                    break;
                }
            }
            // cout << gate->getName() << " index_2_1  = " <<gate->index_2_1 << " , " << " index_2_2  = " << gate->index_2_2 << endl;
        }
    }
}

void NetList::check_input_tran_time(Gate *a)
{
    Gate *gate = a;
    unordered_map<string, Wire *> gate_input = gate->getInputs();

    int cnt_ready = 0;
    if (gate->getType() != "INVX1")
    {
        // cout << gate->getName() << endl;
        // cout << "-----------------------------------------------" << endl;
        for (unordered_map<string, Wire *>::const_iterator it = gate_input.begin(); it != gate_input.end(); ++it)
        {
            // cout << it->first << " , " << it->second->getName() << endl;

            if (it->second->isGreaterThan(it->second->transition_delay, 0.00000000))
            {
                cnt_ready = cnt_ready + 1;
                // cout << gate->getName() << " , " << it->second->getName() << " = " << it->second->transition_delay << " , " << it->first << endl;
            }
            else if (it->second->getInput() == nullptr)
            {
                cnt_ready = cnt_ready + 1;
            }
        }
        // cout << "-----------------------------------------------" << endl;

        if (cnt_ready == 2)
        {
            gate->input_tran_time = (gate->isGreaterThan(gate_input["A1"]->total_propagation_delay, gate_input["A2"]->total_propagation_delay)) ? gate->input_tran_time_A1 : gate->input_tran_time_A2;
            gate->input_wire = (gate->isGreaterThan(gate_input["A1"]->total_propagation_delay, gate_input["A2"]->total_propagation_delay)) ? gate_input["A1"] : gate_input["A2"];
            gate->ready = 1;
        }
    }
}

bool NetList::check_cell_delay()
{
    int cnt = 0;
    for (Gate *gate : gates)
    {
        if (gate->done == 1)
        {
            cnt = cnt + 1;
        }
    }

    if (cnt == gates.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

double NetList::Table_Interpolation()
{
    Liberty liberty("test_lib.lib");
    for (Gate *gate : gates)
    {
        if (gate->ready == 1 && gate->done != 1)
        {
            string cells_name = gate->getType();
            int gate_out_value = gate->output_value;
            double C_req = gate->output_load;
            double S_req = gate->input_tran_time;
            double S1 = gate->in_tran_time_vec[gate->index_2_1];
            double S2 = gate->in_tran_time_vec[gate->index_2_2];
            double C1 = gate->output_load_vec[gate->index_1_1];
            double C2 = gate->output_load_vec[gate->index_1_2];
            double P0 = (gate_out_value == 0) ? liberty.cells[cells_name]->pins["ZN"]->timing->fall_transition->values[gate->index_2_1][gate->index_1_1] : liberty.cells[cells_name]->pins["ZN"]->timing->rise_transition->values[gate->index_2_1][gate->index_1_1];
            double P1 = (gate_out_value == 0) ? liberty.cells[cells_name]->pins["ZN"]->timing->fall_transition->values[gate->index_2_2][gate->index_1_2] : liberty.cells[cells_name]->pins["ZN"]->timing->rise_transition->values[gate->index_2_2][gate->index_1_2];
            double P2 = (gate_out_value == 0) ? liberty.cells[cells_name]->pins["ZN"]->timing->fall_transition->values[gate->index_2_2][gate->index_1_1] : liberty.cells[cells_name]->pins["ZN"]->timing->rise_transition->values[gate->index_2_2][gate->index_1_1];
            double P3 = (gate_out_value == 0) ? liberty.cells[cells_name]->pins["ZN"]->timing->fall_transition->values[gate->index_2_1][gate->index_1_2] : liberty.cells[cells_name]->pins["ZN"]->timing->rise_transition->values[gate->index_2_1][gate->index_1_2];
            double A = P0 + (((P2 - P0) / (S2 - S1)) * (S_req - S1));
            double B = P3 + (((P1 - P3) / (S2 - S1)) * (S_req - S1));
            double P_req = A + (((B - A) / (C2 - C1)) * (C_req - C1));

            double P0_propagation = (gate_out_value == 0) ? liberty.cells[cells_name]->pins["ZN"]->timing->cell_fall->values[gate->index_2_1][gate->index_1_1] : liberty.cells[cells_name]->pins["ZN"]->timing->cell_rise->values[gate->index_2_1][gate->index_1_1];
            double P1_propagation = (gate_out_value == 0) ? liberty.cells[cells_name]->pins["ZN"]->timing->cell_fall->values[gate->index_2_2][gate->index_1_2] : liberty.cells[cells_name]->pins["ZN"]->timing->cell_rise->values[gate->index_2_2][gate->index_1_2];
            double P2_propagation = (gate_out_value == 0) ? liberty.cells[cells_name]->pins["ZN"]->timing->cell_fall->values[gate->index_2_2][gate->index_1_1] : liberty.cells[cells_name]->pins["ZN"]->timing->cell_rise->values[gate->index_2_2][gate->index_1_1];
            double P3_propagation = (gate_out_value == 0) ? liberty.cells[cells_name]->pins["ZN"]->timing->cell_fall->values[gate->index_2_1][gate->index_1_2] : liberty.cells[cells_name]->pins["ZN"]->timing->cell_rise->values[gate->index_2_1][gate->index_1_2];
            double A_propagation = P0_propagation + (((P2_propagation - P0_propagation) / (S2 - S1)) * (S_req - S1));
            double B_propagation = P3_propagation + (((P1_propagation - P3_propagation) / (S2 - S1)) * (S_req - S1));
            double P_req_propagation = A_propagation + (((B_propagation - A_propagation) / (C2 - C1)) * (C_req - C1));
            // cout << " ----------------------------------- " << endl;
            // cout << gate->getName() << endl;
            // cout << " ----------------------------------- " << endl;
            // cout << " C_req = "<< fixed << setprecision(10) << C_req << endl;
            // cout << " S_req = "<< fixed << setprecision(10) << S_req << endl;
            // cout << " S1 = "<< fixed << setprecision(10) << S1 << endl;
            // cout << " S2 = "<< fixed << setprecision(10) << S2 << endl;
            // cout << " C1 = "<< fixed << setprecision(10) << C1 << endl;
            // cout << " C2 = "<< fixed << setprecision(10) << C2 << endl;
            // cout << " P0 = "<< fixed << setprecision(10) << P0 << endl;
            // cout << " P1 = "<< fixed << setprecision(10) << P1 << endl;
            // cout << " P2 = "<< fixed << setprecision(10) << P2 << endl;
            // cout << " P3 = "<< fixed << setprecision(10) << P3 << endl;
            // cout << " A = "<< fixed << setprecision(10) << A << endl;
            // cout << " B = "<< fixed << setprecision(10) << B << endl;
            // cout << " P_req = "<< fixed << setprecision(10) << P_req << endl;
            // cout <<" output_transition_time = " << fixed << setprecision(10) << P_req << endl;
            // cout << " ----------------------------------- " << endl;
            // cout <<" propagation_delay_time = "<< fixed << setprecision(10) << P_req_propagation << endl;
            // cout << " ----------------------------------- " << endl;

            gate->output_tran_time = P_req;
            gate->propagation_delay = P_req_propagation;
            gate->done = 1;
            Wire *output_wire = gate->getOutput();
            Wire *input_wire = gate->input_wire;
            gate->total_propagation_delay = input_wire->total_propagation_delay + P_req_propagation;
            output_wire->total_propagation_delay = input_wire->total_propagation_delay + P_req_propagation + 0.005;
            output_wire->transition_delay = P_req;
            output_wire->pre_gate_propagation_delay = P_req_propagation;
            output_wire->wire_ready = 1;
            // cout << output_wire->getName() << " , " << output_wire->transition_delay << endl;
        }
    }
}

void NetList::sort_delay()
{
    sorted_delay_gates = gates;
    sorted_load_gates = gates;

    auto compare_delay = [](Gate *a, Gate *b)
    {
        return a->propagation_delay < b->propagation_delay;
    };
    sort(sorted_delay_gates.rbegin(), sorted_delay_gates.rend(), compare_delay);
    //for(vector<Gate *>::iterator gate = sorted_delay_gates.begin(); gate != sorted_delay_gates.end(); ++gate)
    //{
    //    cout << (*gate)->getName() << "  propagation_delay =  " << (*gate)->propagation_delay << endl;
    //}

    auto compare_load = [](Gate *a, Gate *b)
    {
        return a->output_load < b->output_load;
    };
    sort(sorted_load_gates.rbegin(), sorted_load_gates.rend(), compare_load);
    //for(vector<Gate *>::iterator gate = sorted_load_gates.begin(); gate != sorted_load_gates.end(); ++gate)
    //{
    //     cout << (*gate)->getName() << "  output_load =  " << (*gate)->output_load << endl;
    //}
}

void NetList::find_path()
{
    sorted_propagation_delay_gates = gates;
    auto compare_load = [](Gate *a, Gate *b)
    {
        return a->total_propagation_delay > b->total_propagation_delay;
    };
    sort(sorted_propagation_delay_gates.begin(), sorted_propagation_delay_gates.end(), compare_load);
    
    //for(vector<Gate *>::iterator gate = sorted_propagation_delay_gates.begin(); gate != sorted_propagation_delay_gates.end(); ++gate)
    //{
    //    cout << (*gate)->getName() << "  propagation_delay =  " << (*gate)->total_propagation_delay << endl;
    //}


    for (vector<Gate *>::iterator gate = sorted_propagation_delay_gates.begin(); gate != sorted_propagation_delay_gates.end(); ++gate)
    {
        
        for(Wire * output: outputs)
        {
            if ((*gate)->getOutput() == output)
            {
                output_gates.push_back((*gate));
            }
        }
        
    }

    Gate *L_path_begin = output_gates.front();
    long_path_gates.push_back(L_path_begin);
    find_longest_path(L_path_begin);

    reverse(long_path_gates.begin(), long_path_gates.end());

    Gate *S_path_end = output_gates.back();
    smallest_path_gates.push_back(S_path_end);
    find_smallest_path(S_path_end);

    reverse(smallest_path_gates.begin(), smallest_path_gates.end());
    
    //ofstream fout_delay("test");
    //
    //for(vector<Gate *>::iterator gate = long_path_gates.begin(); gate != long_path_gates.end(); ++gate)
    //{
    //    fout_delay << (*gate)->getName() << " wires name = "<< (*gate)->getOutput()->getName() << endl;
    //}
    //fout_delay << "--------------------------------------------------" << endl;
    //
    //for(vector<Gate *>::iterator gate = smallest_path_gates.begin(); gate != smallest_path_gates.end(); ++gate)
    //{
    //    fout_delay << (*gate)->getName() << " wires name = "<< (*gate)->getOutput()->getName() << endl;
    //}
    //fout_delay << "--------------------------------------------------" << endl;
    
}

void NetList::find_longest_path(Gate *a)
{
    unordered_map<string, Wire *> gate_input = a->getInputs();
    vector<Wire *> pre_wires;
    for (unordered_map<string, Wire *>::iterator it = gate_input.begin(); it != gate_input.end(); ++it)
    {
        pre_wires.push_back(it->second);
    }
    auto compare_delay = [](Wire *a, Wire *b)
    {
        return a->total_propagation_delay < b->total_propagation_delay;
    };
    sort(pre_wires.rbegin(), pre_wires.rend(), compare_delay);

    Wire *pre_wire = pre_wires.front();
    for (vector<Wire *>::iterator wire = inputs.begin(); wire != inputs.end(); ++wire)
    {
        if ((*wire) == pre_wire)
        {
            return;
        }
    }
    Gate *b = pre_wire->getInput();
    long_path_gates.push_back(b);
    find_longest_path(b);
}

void NetList::find_smallest_path(Gate *a)
{
    unordered_map<string, Wire *> gate_input = a->getInputs();
    vector<Wire *> pre_wires;
    for (unordered_map<string, Wire *>::iterator it = gate_input.begin(); it != gate_input.end(); ++it)
    {
        pre_wires.push_back(it->second);
    }
    auto compare_delay = [](Wire *a, Wire *b)
    {
        return a->total_propagation_delay < b->total_propagation_delay;
    };
    sort(pre_wires.rbegin(), pre_wires.rend(), compare_delay);

    Wire *pre_wire = pre_wires.front();

    for (vector<Wire *>::iterator wire = inputs.begin(); wire != inputs.end(); ++wire)
    {
        if ((*wire) == pre_wire)
        {
            return;
        }
    }
    Gate *b = pre_wire->getInput();
    smallest_path_gates.push_back(b);
    find_smallest_path(b);
}
