#include <iostream>
#include <string>
#include "NetList.h"
#include "Liberty.h"
#include <iomanip>
#include <algorithm>
#include "Pattern.h"

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
    string inputFileName_2(argv[2]);
    string inputFileName_3(argv[3]);

    string studentId = "310513081";
    string caseName = inputFileName_1.substr(0, inputFileName_1.find('.'));

    string output_file_name = studentId + "_" + caseName;
    NetList netList(inputFileName_1);
    Pattern pattern(inputFileName_2);
    Liberty liberty(inputFileName_3);

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
    //netList.clear_ouput_load();
    netList.set_Output_load();
    

    //netList.output_load_vec = liberty.lu_table_template->index_1;
    //netList.in_tran_time_vec = liberty.lu_table_template->index_2;
    

    //for(int i = 0; i < netList.output_load_vec.size(); i++)
    //{
    //    cout << fixed << setprecision(6) << netList.output_load_vec[i] << endl;
    //}

    netList.find_index_1();
    ofstream fout_load(output_file_name + "_load.txt");
    ofstream fout_info(output_file_name + "_gate_info.txt");
    ofstream fout_power(output_file_name + "_gate_power.txt");
    ofstream fout_toggle(output_file_name + "_coverage.txt");
    
    for(int i = 0; i < pattern.data.size(); i++)
    {
        netList.set_input_pattern(pattern.data[i], pattern.indices);
        netList.set_initial_gate_value();
        netList.clear_wire_transition();
        //for(int i = 0; i < pattern.data.size(); i++)
        //{
        //    netList.forward(pattern.data[i], pattern.indices);
        //    for (Gate *gate : netList.getGates())
        //    {    
        //        cout << gate->getName() << " " << gate->value << endl;
        //    }
        //}
    

       

        while(netList.check_cell_delay() == 0)
        {
        //for(int i = 0; i < 30; i++)
        //{
            netList.set_input_index_2();
            netList.Table_Interpolation();
        //}    
        }

        

        netList.sort_delay();
        netList.power_Table_Interpolation();
        netList.calculate_toggle();    
        //netList.find_path();
    
        

        if(i == 0)
        {
            for(vector<Gate *>::iterator gate = netList.sorted_load_gates.begin(); gate != netList.sorted_load_gates.end(); ++gate)
            {
                fout_load << (*gate)->getName() << " " << fixed << setprecision(6) << (*gate)->output_load << endl;
            }
        }
        for(vector<Gate *>::iterator gate = netList.sorted_delay_gates.begin(); gate != netList.sorted_delay_gates.end(); ++gate)
        {
            fout_info  << (*gate)->getName() << " " << (*gate)->output_value << " " << fixed << setprecision(6) << (*gate)->propagation_delay << " " << fixed << setprecision(6) << (*gate)->output_tran_time << endl;
        }
        for(vector<Gate *>::iterator gate = netList.sorted_delay_gates.begin(); gate != netList.sorted_delay_gates.end(); ++gate)
        {
            fout_power  << (*gate)->getName() << " " << fixed << setprecision(6) << (*gate)->internal_power << " " << fixed << setprecision(6) << (*gate)->switching_power << endl;
        }
        fout_toggle << i + 1 <<" " << fixed << setprecision(6) << netList.total_power << " " << fixed << setprecision(2) << netList.coverage << "%"<< endl; 
        
        fout_info << " " << endl;
        fout_power << " " << endl;
        fout_toggle << " " << endl;

        netList.clear_parameters();
    }
    
    //unordered_map<string, Wire *> input_wires_long = netList.long_path_gates[0]->getInputs();
    //unordered_map<string, Wire *> input_wires_short = netList.new_smallest_path_gates[0]->getInputs();
    //double longest_delay; 
    //double shortest_delay;
    //if(netList.long_path_gates[0]->getType() == "INVX1")
    //{
    //    ofstream fout_path(output_file_name + "_path.txt");
    //    
    //    for(int i = 0; i < netList.long_path_gates.size(); i++)
    //    {
    //        netList.long_path_gates_wire.push_back(netList.long_path_gates[i]->getOutput());
    //        longest_delay = netList.long_path_gates[i]->getOutput()->total_propagation_delay;
    //    }
    //    
    //    fout_path << "Longest delay = " << fixed << setprecision(6) << (longest_delay - 0.005) << ", the path is: " << input_wires_long["I"]->getName();
    //    for(int i = 0; i < netList.long_path_gates_wire.size(); ++i)
    //    {
    //        fout_path << " -> " << netList.long_path_gates_wire[i]->getName();
    //    }
    //    fout_path << endl;
    //
    //    for(int i = 0; i < netList.new_smallest_path_gates.size(); i++)
    //    {
    //        netList.smallest_path_gates_wire.push_back(netList.new_smallest_path_gates[i]->getOutput());
    //        shortest_delay = netList.new_smallest_path_gates[i]->getOutput()->total_propagation_delay;
    //    }
    //    
    //    fout_path << "Shortest delay = " << fixed << setprecision(6) << (shortest_delay - 0.005) << ", the path is: " << input_wires_short["I"]->getName();
    //    for(int i = 0; i < netList.smallest_path_gates_wire.size(); ++i)
    //    {
    //        fout_path << " -> " << netList.smallest_path_gates_wire[i]->getName();
    //    }
    //    fout_path << endl;
    //}
    //else if(netList.long_path_gates[0]->getType() != "INVX1")
    //{
    //    ofstream fout_path_1(output_file_name + "_path_1.txt");
    //    ofstream fout_path_2(output_file_name + "_path_2.txt");
    //    for(int i = 0; i < netList.long_path_gates.size(); i++)
    //    {
    //        netList.long_path_gates_wire.push_back(netList.long_path_gates[i]->getOutput());
    //        longest_delay = netList.long_path_gates[i]->getOutput()->total_propagation_delay;
    //    }
    //    
    //    fout_path_1 << "Longest delay = " << fixed << setprecision(6) << (longest_delay - 0.005) << ", the path is: " << input_wires_long["A1"]->getName();
    //    for(int i = 0; i < netList.long_path_gates_wire.size(); ++i)
    //    {
    //        fout_path_1 << " -> " << netList.long_path_gates_wire[i]->getName();
    //    }
    //    fout_path_1 << endl;
    //
    //    fout_path_2 << "Longest delay = " << fixed << setprecision(6) << (longest_delay - 0.005) << ", the path is: " << input_wires_long["A2"]->getName();
    //    for(int i = 0; i < netList.long_path_gates_wire.size(); ++i)
    //    {
    //        fout_path_2 << " -> " << netList.long_path_gates_wire[i]->getName();
    //    }
    //    fout_path_2 << endl;
    //
    //    for(int i = 0; i < netList.new_smallest_path_gates.size(); i++)
    //    {
    //        netList.smallest_path_gates_wire.push_back(netList.new_smallest_path_gates[i]->getOutput());
    //        shortest_delay = netList.new_smallest_path_gates[i]->getOutput()->total_propagation_delay;
    //    }
    //    
    //    fout_path_1 << "Shortest delay = " << fixed << setprecision(6) << (shortest_delay - 0.005) << ", the path is: " << input_wires_short["A1"]->getName();
    //    for(int i = 0; i < netList.smallest_path_gates_wire.size(); ++i)
    //    {
    //        fout_path_1 << " -> " << netList.smallest_path_gates_wire[i]->getName();
    //    }
    //    fout_path_1 << endl;
    //
    //    fout_path_2 << "Shortest delay = " << fixed << setprecision(6) << (shortest_delay - 0.005) << ", the path is: " << input_wires_short["A2"]->getName();
    //    for(int i = 0; i < netList.smallest_path_gates_wire.size(); ++i)
    //    {
    //        fout_path_2 << " -> " << netList.smallest_path_gates_wire[i]->getName();
    //    }
    //    fout_path_2 << endl;
    //}

        
    
    return 0;
}