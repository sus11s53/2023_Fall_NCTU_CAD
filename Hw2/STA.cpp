#include <iostream>
#include <string>
#include "NetList.h"
#include "Liberty.h"
#include <iomanip>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Wrong number of arguments. Please execute as follow : \n"
             << "./hw2 <input_file name> <output_file name>" << endl;
        exit(-1);
    }
    string inputFileName_1(argv[1]);
    string inputFileName_2(argv[3]);

    string studentId = "310513081";
    string caseName = inputFileName_1.substr(0, inputFileName_1.find('.'));

    string output_file_name = studentId + "_" + caseName;
    NetList netList(inputFileName_1);
    Liberty liberty(inputFileName_2);

    //for (const Wire *wire : netList.getWires())
    //{
    //    wire->print();
    //}
    //for (const Gate *gate : netList.getGates())
    //{
    //    gate->print();
    //}
    
    //netList.print();
    
    netList.find_Output_load_path();
    netList.clear_ouput_load();
    netList.set_Output_load();
    netList.set_Output_value();
    netList.find_index_1();
    netList.clear_wire_transition();
    
    
    while(netList.check_cell_delay() == 0)
    {
    //for(int i = 0; i < 30; i++)
    //{
        netList.set_input_index_2();
        netList.Table_Interpolation();
    //}    
    
    }

    netList.sort_delay();    
    netList.find_path();
    
    
    
    ofstream fout_load(output_file_name + "_load.txt");
    for(vector<Gate *>::iterator gate = netList.sorted_load_gates.begin(); gate != netList.sorted_load_gates.end(); ++gate)
    {
        fout_load << (*gate)->getName() << " " << fixed << setprecision(6) << (*gate)->output_load << endl;
    }
    ofstream fout_delay(output_file_name + "_delay.txt");
    for(vector<Gate *>::iterator gate = netList.sorted_delay_gates.begin(); gate != netList.sorted_delay_gates.end(); ++gate)
    {
        fout_delay  << (*gate)->getName() << " " << (*gate)->output_value << " " << fixed << setprecision(6) << (*gate)->propagation_delay << " " << fixed << setprecision(6) << (*gate)->output_tran_time << endl;
    }
    
    
    
    unordered_map<string, Wire *> input_wires_long = netList.long_path_gates[0]->getInputs();
    unordered_map<string, Wire *> input_wires_short = netList.smallest_path_gates[0]->getInputs();
    double longest_delay; 
    double shortest_delay;
    ofstream fout_path(output_file_name + "_path.txt");
    if(netList.long_path_gates[0]->getType() == "INVX1")
    {
        for(int i = 0; i < netList.long_path_gates.size(); i++)
        {
            netList.long_path_gates_wire.push_back(netList.long_path_gates[i]->getOutput());
            longest_delay = netList.long_path_gates[i]->getOutput()->total_propagation_delay;
        }

        fout_path << "Longest delay = " << fixed << setprecision(6) << (longest_delay - 0.005) << ", the path is: " << input_wires_long["I"]->getName();
        for(int i = 0; i < netList.long_path_gates_wire.size(); ++i)
        {
            fout_path << " -> " << netList.long_path_gates_wire[i]->getName();
        }
        fout_path << endl;
    }
    else if(netList.long_path_gates[0]->getType() != "INVX1")
    {     
        for(int i = 0; i < netList.long_path_gates.size(); i++)
        {
            netList.long_path_gates_wire.push_back(netList.long_path_gates[i]->getOutput());
            longest_delay = netList.long_path_gates[i]->getOutput()->total_propagation_delay;
        }
        
        fout_path << "Longest delay = " << fixed << setprecision(6) << (longest_delay - 0.005) << ", the path is: " << input_wires_long["A1"]->getName();
        for(int i = 0; i < netList.long_path_gates_wire.size(); ++i)
        {
            fout_path << " -> " << netList.long_path_gates_wire[i]->getName();
        }
        fout_path << endl;
    }

    if(netList.smallest_path_gates[0]->getType() == "INVX1")
    {
    
        for(int i = 0; i < netList.smallest_path_gates.size(); i++)
        {
            netList.smallest_path_gates_wire.push_back(netList.smallest_path_gates[i]->getOutput());
            shortest_delay = netList.smallest_path_gates[i]->getOutput()->total_propagation_delay;
        }
        
        fout_path << "Shortest delay = " << fixed << setprecision(6) << (shortest_delay - 0.005) << ", the path is: " << input_wires_short["I"]->getName();
        for(int i = 0; i < netList.smallest_path_gates_wire.size(); ++i)
        {
            fout_path << " -> " << netList.smallest_path_gates_wire[i]->getName();
        }
        fout_path << endl;
    }
    
    else if(netList.smallest_path_gates[0]->getType() != "INVX1")
    {     
        for(int i = 0; i < netList.smallest_path_gates.size(); i++)
        {
            netList.smallest_path_gates_wire.push_back(netList.smallest_path_gates[i]->getOutput());
            shortest_delay = netList.smallest_path_gates[i]->getOutput()->total_propagation_delay;
        }
        
        fout_path << "Shortest delay = " << fixed << setprecision(6) << (shortest_delay - 0.005) << ", the path is: " << input_wires_short["A1"]->getName();
        for(int i = 0; i < netList.smallest_path_gates_wire.size(); ++i)
        {
            fout_path << " -> " << netList.smallest_path_gates_wire[i]->getName();
        }
        fout_path << endl;
    }

        
    
    return 0;
}
