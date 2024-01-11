#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;


class Imp
{
    public:
           int mvalue;
           string bin_min;
           bool covered;
           bool sec_covered;
           int one_group;
           int first_merge_value [2];
           int second_merge_value [4];
           int third_merge_value [8];
           bool same_imp;
           int app_times;
           int prime;
           bool catched;
           bool picked;
           bool onsetnum;
           int weight;
           int Import;


           Imp()
           {
                Import = 0;
                weight = -1;
                covered = false;
                sec_covered = false;
                same_imp = false;
                app_times = 0;
                first_merge_value [0] = -1;
                first_merge_value [1] = -1;
                second_merge_value[0] = -1;
                second_merge_value[1] = -1;
                second_merge_value[2] = -1;
                second_merge_value[3] = -1;
                third_merge_value[0] = -1;
                third_merge_value[1] = -1;
                third_merge_value[2] = -1;
                third_merge_value[3] = -1;
                third_merge_value[4] = -1;
                third_merge_value[5] = -1;
                third_merge_value[6] = -1;
                third_merge_value[7] = -1;
                prime = 0;
                catched = false;
                picked = false;
           }

};

//class of Quine–McCluskey algorithm
class QM 
{
    public:
            int num_var {};
            bool weight_three;
            QM()
            {
                weight_three = false;
            }
            vector<Imp> minterm_value;
            vector<Imp> on_set_min;
            vector<Imp> dc_set_min;

            vector<vector<Imp> > or_col;
            vector<vector<Imp> > first_col;
            vector<vector<Imp> > second_col;
            vector<vector<Imp> > third_col;
            vector<Imp> unpro_prime_imp;
            vector<Imp> final_imp;
            vector<Imp> old_final_imp;
            vector<Imp> new_final_imp;
            
            int count_num_one (Imp);
            string dectobin(int);
            bool onediff(Imp , Imp);
            bool group_diff_one(Imp , Imp);
            void first_merge();
            void second_merge();
            void third_merge();
            Imp merge2(Imp , Imp);
            Imp merge4(Imp , Imp);
            Imp merge8(Imp , Imp);
            bool getinput();
            void print_ori_imp();
            void print_first_merge();
            void print_second_merge();
            void print_third_merge();
            void or_classify();
            bool check_same_imp(Imp , Imp);
            void record_same_imp_sec();
            void record_same_imp_thir();
            void do_unpro_prime_imp();
            void print_unpro();

            //Petrick's Method
            void appear_times();
            void find_prime(int);
            void old_find_prime();
            void exclude_prime();
            void catch_num();
            bool check_catch();
            void old_final_prime_imp();
            void final_prime_imp();

            //print output
            void print_sort(vector<Imp>);

            //get input
            void Set_Num_Var(int);
            void Add_On_Set(int);
            void Add_DC_Set(int);
            
            void print_catch();
            void reset();
            vector<Imp> literal_compare();
            int Get_app_times(int);
            void Importance_analyze();
            int change2point(int);
            Imp max_point();
            void new_find_prime();
            void catch_num2(Imp);
            vector<Imp> compare_one_num(Imp);
            Imp max_Imp(vector<Imp>);



};

void QM::print_catch()
{
    for(int i = 0; i < minterm_value.size(); i++)
    {
        cout << minterm_value[i].mvalue << "  catched = " << minterm_value[i].catched << endl;
    }
}

bool QM::getinput()
{
    cout << " variables numbers: ";
    cin >> num_var;
    or_col.resize(num_var + 1);
    
    int on_set;
    cout << "on_set sep by space (-1 is end)";
    while(cin >> on_set)
    {
        if(on_set == -1)
        {
            break;
        }
        Imp min_value;
        min_value.mvalue = on_set;
        min_value.onsetnum = true;
        minterm_value.push_back(min_value);
    }
    
    int dc_set;
    cout << "dc_set sep by space (-1 is end)";
    while(cin >> dc_set)
    {
        if(dc_set == -1)
        {
            break;
        }
        Imp min_value;
        min_value.mvalue = dc_set;
        min_value.onsetnum = false;
        minterm_value.push_back(min_value);
    }

    if(dc_set == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void QM::Set_Num_Var(int a)
{
    num_var = a;
    or_col.resize(num_var + 1);
}

void QM::Add_On_Set(int a)
{
    int on_set = a;

    Imp min_value;
    min_value.mvalue = on_set;
    min_value.onsetnum = true;
    minterm_value.push_back(min_value);
}

void QM::Add_DC_Set(int a)
{
    int dc_set = a;
    
    Imp min_value;
    min_value.mvalue = dc_set;
    min_value.onsetnum = false;
    minterm_value.push_back(min_value);
    dc_set_min.push_back(min_value);
}


void Init(ifstream &fin, QM &QM) {
	// input from a file
	char c;
	while( fin >> c ) {
		if( c == '.' )	
			continue;
		if( c == 'i' ) 
        {
			int n;
			fin >> n ;
			QM.Set_Num_Var(n);
		}
		else if( c == 'm' ) 
        {
			string s;
			stringstream ss;
			getline( fin , s );
			getline( fin , s );
			ss << s ;
			int n;
			while( ss >> n ) 
				QM.Add_On_Set(n);
		}
		else if( c =='d' ) 
        {
			string s;
			stringstream ss;
			getline( fin , s );
			getline( fin , s );
			ss << s ;
			int n;
			while( ss >> n ) 
				QM.Add_DC_Set(n);
		}
		else {
			cerr<<"Wrong input format."<< endl;
			exit(-1);
		}
	}
}



string QM::dectobin(int a)
{
    string s;
    
    //decimal turn to binary
    while(a > 0){
        s.push_back('0'+(a % 2));
        a = a >> 1;
    }

    //fill 0 after turn
    int fill_0 = (num_var > s.length()) ? num_var - s.length():0;
    s = s.append(fill_0 ,'0');

    //reverse the whole string 
    reverse(s.begin() , s.end());

    return s;
}

int QM::count_num_one(Imp a)
{
    int one_num = 0;
    string b = a.bin_min;
    one_num = count(b.begin(), b.end(), '1');
    return one_num;
}

bool QM::group_diff_one(Imp a , Imp b)
{
    if(a.one_group == b.one_group + 1 || a.one_group == b.one_group - 1)
    {
        return true;
    }
    
    return false;
}

bool QM::onediff(Imp a , Imp b)
{   
    if(group_diff_one(a,b) == false)
    {
        return false;
    }
    int diff_num = 0;
    for(int i = 0; i < a.bin_min.size(); i++)
    {
        if(a.bin_min[i] != b.bin_min[i] )
        {
            diff_num = diff_num + 1;
        }

        if(diff_num > 1)
        {
            return false;
        }
    }
    return true;
}

Imp QM::merge2(Imp a , Imp b)
{
    for(int i = 0; i < a.bin_min.size(); i++)
    {
        if(a.bin_min[i] != b.bin_min[i] )
        {
            a.bin_min[i] = '-';
        }

    }
    a.first_merge_value[0] = a.mvalue;
    a.first_merge_value[1] = b.mvalue;

    for(int j = 0; j < minterm_value.size(); j++)
    {
        if(a.mvalue == minterm_value[j].mvalue)
        {
            minterm_value[j].catched = true;
        }

        if(b.mvalue == minterm_value[j].mvalue)
        {
            minterm_value[j].catched = true;
        }
    }
    return a;
}

Imp QM::merge4(Imp a , Imp b)
{
    for(int i = 0; i < a.bin_min.size(); i++)
    {
        if(a.bin_min[i] != b.bin_min[i] )
        {
            a.bin_min[i] = '-';
        }

    }
    a.second_merge_value[0] = a.first_merge_value[0];
    a.second_merge_value[1] = a.first_merge_value[1];
    a.second_merge_value[2] = b.first_merge_value[0];
    a.second_merge_value[3] = b.first_merge_value[1];
    return a;
}

Imp QM::merge8(Imp a , Imp b)
{
    for(int i = 0; i < a.bin_min.size(); i++)
    {
        if(a.bin_min[i] != b.bin_min[i] )
        {
            a.bin_min[i] = '-';
        }
    }
    a.third_merge_value[0] = a.second_merge_value[0];
    a.third_merge_value[1] = a.second_merge_value[1];
    a.third_merge_value[2] = a.second_merge_value[2];
    a.third_merge_value[3] = a.second_merge_value[3];
    a.third_merge_value[4] = b.second_merge_value[0];
    a.third_merge_value[5] = b.second_merge_value[1];
    a.third_merge_value[6] = b.second_merge_value[2];
    a.third_merge_value[7] = b.second_merge_value[3];
    return a;
}

void QM::first_merge()
{
    bool able;
    Imp y;
    first_col.resize(num_var);
    for(int i = 0; i < num_var + 1; i++)
    {
        for(int j = i + 1; j < num_var + 1; j++)
        {
            for(int k = 0; k < or_col[i].size(); k++)
            {
                for(int l = 0; l < or_col[j].size(); l++)
                {   
                    able = onediff(or_col[i][k] , or_col[j][l]);
                    if(able == true)
                    {
                        y = merge2(or_col[i][k] , or_col[j][l]);
                        y.one_group = count_num_one(y);
                        first_col[y.one_group].push_back(y);
                    }
                }
            }
        }
    }
    //print_first_merge();
}

void QM::second_merge()
{
    bool able;
    Imp y;
    second_col.resize(num_var - 1);
    for(int i = 0; i < num_var; i++)
    {
        for(int j = i + 1; j < num_var; j++)
        {
            for(int k = 0; k < first_col[i].size(); k++)
            {
                for(int l = 0; l < first_col[j].size(); l++)
                {   
                    able = onediff(first_col[i][k] , first_col[j][l]);
                    if(able == true)
                    {
                        first_col[i][k].covered = true;
                        first_col[j][l].covered = true;
                        y = merge4(first_col[i][k] , first_col[j][l]);
                        y.one_group = count_num_one(y);
                        second_col[y.one_group].push_back(y);
                    }
                }
            }
        }
    }
    record_same_imp_sec();
    //print_second_merge();
}

void QM::third_merge()
{
    bool able = false;
    Imp y;
    third_col.resize(num_var - 2);
    for(int i = 0; i < num_var - 1; i++)
    {
        for(int j = i + 1; j < num_var - 1; j++)
        {
            for(int k = 0; k < second_col[i].size(); k++)
            {
                for(int l = 0; l < second_col[j].size(); l++)
                {   
                    if(second_col[i][k].same_imp == false && second_col[j][l].same_imp == false)
                    {
                        able = onediff(second_col[i][k] , second_col[j][l]);
                        if(able == true)
                        {
                            second_col[i][k].sec_covered = true;
                            second_col[j][l].sec_covered = true;
                            y = merge8(second_col[i][k] , second_col[j][l]);
                            y.one_group = count_num_one(y);
                            third_col[y.one_group].push_back(y);
                        }
                    }
                }
            }
        }
    }
    record_same_imp_thir();
    //print_third_merge();
}

void QM::print_ori_imp()
{
    for(int i = 0; i <= num_var; i++)
    {
        for(int j = 0; j < or_col[i].size(); j++)
        {
            cout << or_col[i][j].mvalue << " group " << i <<" " << or_col[i][j].bin_min << endl;
        }
    }
}

void QM::print_first_merge()
{
    for(int i = 0; i < first_col.size(); i++)
    {
        for(int j = 0; j < first_col[i].size(); j++)
        {
            cout << "(" << first_col[i][j].first_merge_value[0] << "," <<first_col[i][j].first_merge_value[1] <<")" << " group " << i << " "<< first_col[i][j].bin_min << endl;
        }
    }

}

void QM::print_second_merge()
{
    for(int i = 0; i < second_col.size(); i++)
    {
        for(int j = 0; j < second_col[i].size(); j++)
        {
            cout << "(" << second_col[i][j].second_merge_value[0] << "," <<second_col[i][j].second_merge_value[1] <<"," <<second_col[i][j].second_merge_value[2] << "," <<second_col[i][j].second_merge_value[3] << ")" << " group " << i << " "<< second_col[i][j].bin_min << " same: " << second_col[i][j].same_imp << endl;
        }
    }

}

void QM::print_third_merge()
{
    for(int i = 0; i < third_col.size(); i++)
    {
        for(int j = 0; j < third_col[i].size(); j++)
        {
            cout << "(" << third_col[i][j].third_merge_value[0] << "," <<third_col[i][j].third_merge_value[1] <<"," <<third_col[i][j].third_merge_value[2] << "," <<third_col[i][j].third_merge_value[3] << "," << third_col[i][j].third_merge_value[4] << "," <<third_col[i][j].third_merge_value[5] <<"," <<third_col[i][j].third_merge_value[6] << "," <<third_col[i][j].third_merge_value[7] << ")" << " group " << i << " "<< third_col[i][j].bin_min << " same: " << third_col[i][j].same_imp << endl;
        }
    }

}

bool QM::check_same_imp(Imp a , Imp b)
{
    int same_bit = 0;

    for(int i = 0; i < a.bin_min.size(); i++)
    {
        if(a.bin_min[i] == b.bin_min[i] )
        {
            same_bit = same_bit + 1;
        }
    }
    if(same_bit == num_var)
    {
        return true;
    }
    return false;
}

void QM::record_same_imp_sec()
{
    bool mark;
    for(int i = 0; i < num_var - 1; i++)
    {
        for(int j = 0; j < second_col[i].size(); j++)
        {
            for(int k = j + 1; k < second_col[i].size(); k++)
            {
                if(second_col[i][j].same_imp != true && second_col[i][k].same_imp != true)
                {
                        mark = check_same_imp(second_col[i][j],second_col[i][k]);
                        if(mark == true)
                        {
                            second_col[i][j].same_imp = false;
                            second_col[i][k].same_imp = true; 
                        }
                }
            }
        }
    }
}

void QM::record_same_imp_thir()
{
    bool mark;
    for(int i = 0; i < num_var - 2; i++)
    {
        for(int j = 0; j < third_col[i].size(); j++)
        {
            for(int k = j + 1; k < third_col[i].size(); k++)
            {
                if(third_col[i][j].same_imp != true && third_col[i][k].same_imp != true)
                {
                        mark = check_same_imp(third_col[i][j],third_col[i][k]);
                        if(mark == true)
                        {
                            third_col[i][j].same_imp = false;
                            third_col[i][k].same_imp = true; 
                        }
                }
            }
        }
    }
}

void QM::do_unpro_prime_imp()
{
    for(int i = 0; i < minterm_value.size(); i++)
    {
        if(minterm_value[i].catched == false)
        {
            minterm_value[i].bin_min = dectobin(minterm_value[i].mvalue);
            unpro_prime_imp.push_back(minterm_value[i]);
            minterm_value[i].catched = true;
            minterm_value[i].app_times = 1;
            //cout << minterm_value[i].mvalue << " " << minterm_value[i].bin_min << endl;
        }
    }

    for(int k = 0; k < num_var; k++)
    {
        for(int l = 0; l < first_col[k].size(); l++)
        {
            if(first_col[k][l].covered == false)
            {
                unpro_prime_imp.push_back(first_col[k][l]);
                //cout << first_col[k][l].first_merge_value[0] << " , "<<first_col[k][l].first_merge_value[1]  << " " << first_col[k][l].bin_min << endl;
            }
        }
    }

    for(int i = 0; i < num_var - 1; i++)
    {
        for(int j = 0; j < second_col[i].size(); j++)
        {
            if(num_var == 8)
            {
                if(second_col[i][j].sec_covered == false && second_col[i][j].same_imp == false)
                {
                    unpro_prime_imp.push_back(second_col[i][j]);
                }
            }
            else
            {
                if(second_col[i][j].same_imp == false)
                {
                    unpro_prime_imp.push_back(second_col[i][j]);
                    //cout << second_col[i][j].second_merge_value[0] << " , "<< second_col[i][j].second_merge_value[1] << " , " << second_col[i][j].second_merge_value[2] << " , " << second_col[i][j].second_merge_value[3] << " " << second_col[i][j].bin_min << endl;
                }
            }
        }
    }

    if(num_var == 8)
    {
        for(int i = 0; i < num_var - 2; i++)
        {
            for(int j = 0; j < third_col[i].size(); j++)
            {   
                    if(third_col[i][j].same_imp == false)
                    {
                        unpro_prime_imp.push_back(third_col[i][j]);
                    }
            
            }
        }
    }
    
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        if(unpro_prime_imp[i].first_merge_value[1] == -1)
        {
            unpro_prime_imp[i].weight = 0;
        }
        else if(unpro_prime_imp[i].second_merge_value[1] == -1)
        {
            unpro_prime_imp[i].weight = 1;
        }
        else if(unpro_prime_imp[i].third_merge_value[1] == -1)
        {
            unpro_prime_imp[i].weight = 2;
        }
        else
        {
            unpro_prime_imp[i].weight = 3;
            weight_three = true;
        } 
    }

    //print_catch();
    //print_unpro();
    //print_sort(unpro_prime_imp);
}

void QM::print_unpro()
{
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        cout << unpro_prime_imp[i].bin_min << " weight = "<< unpro_prime_imp[i].weight << endl; 
    }
    cout << unpro_prime_imp.size() << endl;
}

//void QM::print_sort(vector<Imp> b)
//{
//    vector<string> outputs{};
//    for (const Imp &imp : b)
//    {
//        outputs.emplace_back(imp.bin_min);
//    }
//    sort(outputs.rbegin(), outputs.rend());
//    for (const string &output : outputs)
//    {
//        for (int i = 0; i < output.size(); i++)
//        {
//            if (output[i] == '1')
//            {    
//                cout << (char)('A' + i);
//            }
//            else if (output[i] == '0')
//            {
//                cout << (char)('A' + i) << "'";
//            }
//        }
//        cout << endl;
//    }
//}


void QM::or_classify()
{   Imp a;
    for(int i = 0; i < minterm_value.size(); i++)
    {   
        a.mvalue = minterm_value[i].mvalue;
        a.bin_min = dectobin(minterm_value[i].mvalue);
        a.one_group = count_num_one(a);
        or_col[a.one_group].push_back(a); 
    }
    //print_ori_imp();
}



void QM::appear_times()
{
    for(int x = 0; x < minterm_value.size(); x++)
    {
        for(int k = 0; k < num_var; k++)
        {
            for(int l = 0; l < first_col[k].size(); l++)
            {
                if(first_col[k][l].covered == false)
                {
                    if(minterm_value[x].mvalue == first_col[k][l].first_merge_value[0] || minterm_value[x].mvalue == first_col[k][l].first_merge_value[1])
                    {
                        minterm_value[x].app_times = minterm_value[x].app_times + 1;
                    }
                }
            }
        }

        for(int i = 0; i < num_var - 1; i++)
        {
            for(int j = 0; j < second_col[i].size(); j++)
            {
                if(num_var == 8)
                {
                    if(second_col[i][j].sec_covered == false && second_col[i][j].same_imp == false)
                    {
                        if(minterm_value[x].mvalue == second_col[i][j].second_merge_value[0] || minterm_value[x].mvalue == second_col[i][j].second_merge_value[1] || minterm_value[x].mvalue == second_col[i][j].second_merge_value[2] || minterm_value[x].mvalue == second_col[i][j].second_merge_value[3])
                        {
                            minterm_value[x].app_times = minterm_value[x].app_times + 1;
                        }
                    }
                }
                else
                {
                    if(second_col[i][j].same_imp == false)
                    {
                        if(minterm_value[x].mvalue == second_col[i][j].second_merge_value[0] || minterm_value[x].mvalue == second_col[i][j].second_merge_value[1] || minterm_value[x].mvalue == second_col[i][j].second_merge_value[2] || minterm_value[x].mvalue == second_col[i][j].second_merge_value[3])
                        {
                            minterm_value[x].app_times = minterm_value[x].app_times + 1;
                        }
                    }
                }
            }
        }
        if(num_var == 8)
        {
            for(int k = 0; k < num_var - 2; k++)
            {
                for(int l = 0; l < third_col[k].size(); l++)
                {
                    if(third_col[k][l].same_imp == false)
                    {
                        if(minterm_value[x].mvalue == third_col[k][l].third_merge_value[0] || minterm_value[x].mvalue == third_col[k][l].third_merge_value[1]|| minterm_value[x].mvalue == third_col[k][l].third_merge_value[2] || minterm_value[x].mvalue == third_col[k][l].third_merge_value[3] || minterm_value[x].mvalue == third_col[k][l].third_merge_value[4] || minterm_value[x].mvalue == third_col[k][l].third_merge_value[5] || minterm_value[x].mvalue == third_col[k][l].third_merge_value[6] || minterm_value[x].mvalue == third_col[k][l].third_merge_value[7])
                        {
                            minterm_value[x].app_times = minterm_value[x].app_times + 1;
                        }
                    }
                }
            }
        }
        if(minterm_value[x].onsetnum == false)
        {
            minterm_value[x].app_times = 0;
        }
    }

    //for(int y = 0; y < minterm_value.size(); y++)
    //{
    //    cout << minterm_value[y].mvalue << " number is " << minterm_value[y].app_times << endl;
    //}
}

int QM::Get_app_times(int a)
{
    for(int i = 0; i < minterm_value.size();i++)
    {
        if(a == minterm_value[i].mvalue)
        {
            if(minterm_value[i].picked == true)
            {
                return 0;
            }
            else 
            {
                return minterm_value[i].app_times;
            }
        }
    }
}

int QM::change2point(int a)
{
    int point = 0;
    switch(a)
    {
        case 0:
                point = 0;
                break;
        case 1:
                point = 5;
                break;
        case 2:
                point = 4;
                break;
        case 3:
                point = 3;
                break;
        case 4:
                point = 2;
                break;

    }
    return point;
}

Imp QM::max_point()
{
    Imp temp = unpro_prime_imp[0];
    Imp max;
    for(int i = 1; i < unpro_prime_imp.size(); i++)
    {
        max = (temp.Import >= unpro_prime_imp[i].Import) ? (temp) : (unpro_prime_imp[i]);
        temp = max;
    }


    return max;
}

void QM::Importance_analyze()
{  
    // give importance
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        if(unpro_prime_imp[i].second_merge_value[1] == -1)
        {
            unpro_prime_imp[i].Import = change2point(Get_app_times(unpro_prime_imp[i].first_merge_value[0])) + change2point(Get_app_times(unpro_prime_imp[i].first_merge_value[1])) + (unpro_prime_imp[i].weight);
            //cout << unpro_prime_imp[i].first_merge_value[0] << " , " << unpro_prime_imp[i].first_merge_value[1] << " the importance point is " << unpro_prime_imp[i].Import << endl;
        }
        else if(unpro_prime_imp[i].third_merge_value[1] == -1)
        {
            unpro_prime_imp[i].Import = change2point(Get_app_times(unpro_prime_imp[i].second_merge_value[0])) + change2point(Get_app_times(unpro_prime_imp[i].second_merge_value[1])) + change2point(Get_app_times(unpro_prime_imp[i].second_merge_value[2])) + change2point(Get_app_times(unpro_prime_imp[i].second_merge_value[3])) + (unpro_prime_imp[i].weight);
            //cout << unpro_prime_imp[i].second_merge_value[0] << " , " << unpro_prime_imp[i].second_merge_value[1] << " , " << unpro_prime_imp[i].second_merge_value[2] << " , " << unpro_prime_imp[i].second_merge_value[3] << " the importance point is " <<  unpro_prime_imp[i].Import << endl;
        }
        else if(unpro_prime_imp[i].first_merge_value[1] != -1)
        {
            unpro_prime_imp[i].Import = change2point(Get_app_times(unpro_prime_imp[i].third_merge_value[0])) + change2point(Get_app_times(unpro_prime_imp[i].third_merge_value[1])) + change2point(Get_app_times(unpro_prime_imp[i].third_merge_value[2])) + change2point(Get_app_times(unpro_prime_imp[i].third_merge_value[3])) + change2point(Get_app_times(unpro_prime_imp[i].third_merge_value[4])) + change2point(Get_app_times(unpro_prime_imp[i].third_merge_value[5])) + change2point(Get_app_times(unpro_prime_imp[i].third_merge_value[6])) + change2point(Get_app_times(unpro_prime_imp[i].third_merge_value[7])) + (unpro_prime_imp[i].weight);    
            //cout << unpro_prime_imp[i].Import << endl;
        }
    }
}

void QM::new_find_prime()
{
    //find app = 1;
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        for(int j = 0; j < minterm_value.size(); j++)
        {
            if(minterm_value[j].app_times == 1 && minterm_value[j].onsetnum == true && minterm_value[j].picked == false)
            {
                if(unpro_prime_imp[i].first_merge_value[1] == -1)
                {
                    if(unpro_prime_imp[i].mvalue == minterm_value[j].mvalue)
                    {
                        new_final_imp.push_back(unpro_prime_imp[i]);
                        catch_num2(unpro_prime_imp[i]);
                    }
                }
                else if(unpro_prime_imp[i].second_merge_value[1] == -1)
                {
                    if(unpro_prime_imp[i].first_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].first_merge_value[1] == minterm_value[j].mvalue)
                    {
                        new_final_imp.push_back(unpro_prime_imp[i]);
                        catch_num2(unpro_prime_imp[i]);
                    }
                }
                else if(unpro_prime_imp[i].third_merge_value[1] == -1)
                {
                    if(unpro_prime_imp[i].second_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[3] == minterm_value[j].mvalue)
                    {
                        new_final_imp.push_back(unpro_prime_imp[i]);
                        catch_num2(unpro_prime_imp[i]);
                    }
                }
                else
                {
                    if(unpro_prime_imp[i].third_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[3] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[4] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[5] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[6] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[7] == minterm_value[j].mvalue)
                    {
                        new_final_imp.push_back(unpro_prime_imp[i]);
                        catch_num2(unpro_prime_imp[i]);
                    }
                }
            }
        }
    }
    
    //for(int i = 0; i < new_final_imp.size(); i++)
    //{
    //    cout << new_final_imp[i].bin_min << endl;
    //}

    //cout << check_catch() << endl;
    //cout << "////////////////" << endl;
    
    while(!check_catch())
    {
        Importance_analyze();
        Imp max = max_point();
        new_final_imp.push_back(max);
        catch_num2(max);
        //cout << max.bin_min << endl;
        //for(int i = 0; i < unpro_prime_imp.size(); i++)
        //{
        //    if(max.bin_min == unpro_prime_imp[i].bin_min)
        //    {
        //        unpro_prime_imp[i].Import = 0;
        //    }
        //}
        //cout << check_catch() << " aaa "<< endl;
    }

}

bool QM::check_catch()
{
    for(int l = 0; l < minterm_value.size(); l++)
    {
        if(minterm_value[l].picked == false && minterm_value[l].onsetnum == true)
        {
            return false;
        }
    }
    return true;
}

void QM::catch_num2(Imp a)
{
            if(a.first_merge_value[1] == -1)
            {
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(a.mvalue == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                    }
                } 
            }
            else if(a.second_merge_value[1] == -1)
            {
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(a.first_merge_value[0] == minterm_value[j].mvalue || a.first_merge_value[1] == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                    }
                } 
            }
            else if(a.third_merge_value[1] == -1)
            {
                for(int k = 0; k < minterm_value.size(); k++)
                {
                    if(a.second_merge_value[0] == minterm_value[k].mvalue || a.second_merge_value[1] == minterm_value[k].mvalue || a.second_merge_value[2] == minterm_value[k].mvalue || a.second_merge_value[3] == minterm_value[k].mvalue)
                    {
                        minterm_value[k].picked = true;
                    }
                } 
            }
            else
            {
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(a.third_merge_value[0] == minterm_value[j].mvalue || a.third_merge_value[1] == minterm_value[j].mvalue || a.third_merge_value[2] == minterm_value[j].mvalue || a.third_merge_value[3] == minterm_value[j].mvalue || a.third_merge_value[4] == minterm_value[j].mvalue || a.third_merge_value[5] == minterm_value[j].mvalue || a.third_merge_value[6] == minterm_value[j].mvalue || a.third_merge_value[7] == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                    }
                }
            }
}

void QM::find_prime(int a)
{
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        for(int j = 0; j < minterm_value.size(); j++)
        {
            if(minterm_value[j].app_times == a && minterm_value[j].onsetnum == true && minterm_value[j].picked == false)
            {
                if(unpro_prime_imp[i].first_merge_value[1] == -1)
                {
                    if(unpro_prime_imp[i].mvalue == minterm_value[j].mvalue)
                    {
                        unpro_prime_imp[i].prime = 1;
                        //cout << unpro_prime_imp[i].mvalue <<" |||||||||| "<< unpro_prime_imp[i].bin_min << " |||||||||| " << endl;
                        if(a != 1)
                        {
                            minterm_value[j].picked = true;
                            bool b = check_catch();
                            if(b == true)
                            {
                                return;
                            }
                            
                        }
                    }
                }
                else if(unpro_prime_imp[i].second_merge_value[1] == -1)
                {
                    if((unpro_prime_imp[i].first_merge_value[0] == minterm_value[j].mvalue) || (unpro_prime_imp[i].first_merge_value[1] == minterm_value[j].mvalue))
                    {
                        unpro_prime_imp[i].prime = 1;
                        //cout << unpro_prime_imp[i].first_merge_value[0] << " , " << unpro_prime_imp[i].first_merge_value[1] <<" |||||||||| "<< unpro_prime_imp[i].bin_min << " |||||||||| " << endl;
                        if(a != 1)
                        {
                            minterm_value[j].picked = true;
                            bool b = check_catch();
                            if(b == true)
                            {
                                return;
                            }
                        }
                    }
                }
                else if(unpro_prime_imp[i].third_merge_value[1] == -1)
                {
                    if((unpro_prime_imp[i].second_merge_value[0] == minterm_value[j].mvalue) || (unpro_prime_imp[i].second_merge_value[1] == minterm_value[j].mvalue) || (unpro_prime_imp[i].second_merge_value[2] == minterm_value[j].mvalue) || (unpro_prime_imp[i].second_merge_value[3] == minterm_value[j].mvalue))
                    {
                        unpro_prime_imp[i].prime = 1;
                        //cout << unpro_prime_imp[i].second_merge_value[0] << " , " << unpro_prime_imp[i].second_merge_value[1] << " , " << unpro_prime_imp[i].second_merge_value[2] << " , " << unpro_prime_imp[i].second_merge_value[3] <<" |||||||||| "<< unpro_prime_imp[i].bin_min << " |||||||||| " << endl;  
                        if(a != 1)
                        {
                            minterm_value[j].picked = true;
                            bool b = check_catch();
                            if(b == true)
                            {
                                return;
                            }
                        }
                    }
                }
                
                else if(num_var == 8)
                {
                    if(unpro_prime_imp[i].third_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[3] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[4] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[5] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[6] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[7] == minterm_value[j].mvalue)
                    {
                        unpro_prime_imp[i].prime = 1;
                        if(a != 1)
                        {
                            minterm_value[j].picked = true;
                            bool b = check_catch();
                            if(b == true)
                            {
                                return;
                            }
                        }
                    }
                }
            }
        }
        
        if(weight_three == true)
        {
            if(unpro_prime_imp[i].weight == 3 && unpro_prime_imp[i].prime != 1)
            {
                unpro_prime_imp[i].prime = 1;
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(unpro_prime_imp[i].third_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[3] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[4] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[5] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[6] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[7] == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                    }
                }
            }
        }
        else
        {
            if(unpro_prime_imp[i].weight == 2 && unpro_prime_imp[i].prime != 1)
            {
                unpro_prime_imp[i].prime = 1;
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(unpro_prime_imp[i].second_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[3] == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                    }
                }
                //cout << unpro_prime_imp[i].second_merge_value[0] << " , " << unpro_prime_imp[i].second_merge_value[1] << " , " << unpro_prime_imp[i].second_merge_value[2] << " , " << unpro_prime_imp[i].second_merge_value[3] <<" |||||||||| "<< unpro_prime_imp[i].bin_min << " |||||||||| " << endl;
            }
        }
    }

    catch_num();

    //for(int i = 0; i < minterm_value.size();i++)
    //{
    //    if(minterm_value[i].picked == false && minterm_value[i].onsetnum == true)
    //    {
    //        cout << minterm_value[i].mvalue << " has not got it!" << endl;
    //    }
    //}
    
}

void QM::old_find_prime()
{
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        for(int j = 0; j < minterm_value.size(); j++)
        {
            if(minterm_value[j].app_times == 1 && minterm_value[j].picked == false)
            {
                if(unpro_prime_imp[i].first_merge_value[1] == -1)
                {
                    if(unpro_prime_imp[i].mvalue == minterm_value[j].mvalue)
                    {
                        unpro_prime_imp[i].prime = 1;
                        //cout << unpro_prime_imp[i].mvalue << endl;
                    }
                }
                else if(unpro_prime_imp[i].second_merge_value[1] == -1)
                {
                    if(unpro_prime_imp[i].first_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].first_merge_value[1] == minterm_value[j].mvalue)
                    {
                        unpro_prime_imp[i].prime = 1;
                        //cout << unpro_prime_imp[i].first_merge_value[0] << " , " << unpro_prime_imp[i].first_merge_value[1] << endl;
                    }
                }
                else if(unpro_prime_imp[i].third_merge_value[1] == -1)
                {
                    if(unpro_prime_imp[i].second_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[3] == minterm_value[j].mvalue)
                    {
                        unpro_prime_imp[i].prime = 1;
                        //cout << unpro_prime_imp[i].second_merge_value[0] << " , " << unpro_prime_imp[i].second_merge_value[1] << " , " << unpro_prime_imp[i].second_merge_value[2] << " , " << unpro_prime_imp[i].second_merge_value[3] << endl;
                    }
                }
                else
                {
                    if(unpro_prime_imp[i].third_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[3] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[4] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[5] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[6] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[7] == minterm_value[j].mvalue)
                    {
                        unpro_prime_imp[i].prime = 1;
                    }
                }
            }
        }
    }
    catch_num();
    //exclude_prime();
    //old_final_prime_imp();
    for(int j = 0; j < minterm_value.size(); j++)
    {
        if(minterm_value[j].picked == false)
        {
            vector<Imp> tmp_vec;
            Imp tmp;
            tmp_vec = compare_one_num(minterm_value[j]);
            tmp = max_Imp(tmp_vec);
            old_final_imp.push_back(tmp); 
            if(tmp.second_merge_value[1] == -1)
            {
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(tmp.first_merge_value[0] == minterm_value[j].mvalue || tmp.first_merge_value[1] == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                        minterm_value[j].app_times = 0;
                        //cout << minterm_value[j].mvalue << " is picked " << minterm_value[j].app_times << " appear times is " << endl;
                    }
                }
                //cout << tmp.first_merge_value[0] << " , " << tmp.first_merge_value[1] << endl;    
            }
            else if(tmp.third_merge_value[1] == -1)
            {
               for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(tmp.second_merge_value[0] == minterm_value[j].mvalue || tmp.second_merge_value[1] == minterm_value[j].mvalue || tmp.second_merge_value[2] == minterm_value[j].mvalue || tmp.second_merge_value[3] == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                        minterm_value[j].app_times = 0;
                        //cout << minterm_value[j].mvalue << " is picked " << minterm_value[j].app_times << " appear times is " << endl;
                    }
                }
               //cout << tmp.second_merge_value[0] << " , " << tmp.second_merge_value[1] << " , " << tmp.second_merge_value[2] << " , " << tmp.second_merge_value[3] << endl;
            }
        }
    }
}

void QM::catch_num()
{
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        if(unpro_prime_imp[i].prime == 1)
        {
            
            if(unpro_prime_imp[i].first_merge_value[1] == -1)
            {
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(unpro_prime_imp[i].mvalue == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                        minterm_value[j].app_times = 0;
                        //cout << minterm_value[j].mvalue << " is picked " << minterm_value[j].app_times << " appear times is " << endl;
                    }
                } 
            }
            else if(unpro_prime_imp[i].second_merge_value[1] == -1)
            {
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(unpro_prime_imp[i].first_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].first_merge_value[1] == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                        minterm_value[j].app_times = 0;
                        //cout << minterm_value[j].mvalue << " is picked " << minterm_value[j].app_times << " appear times is " << endl;
                    }
                } 
            }
            else if(unpro_prime_imp[i].third_merge_value[1] == -1)
            {
                for(int k = 0; k < minterm_value.size(); k++)
                {
                    if(unpro_prime_imp[i].second_merge_value[0] == minterm_value[k].mvalue || unpro_prime_imp[i].second_merge_value[1] == minterm_value[k].mvalue || unpro_prime_imp[i].second_merge_value[2] == minterm_value[k].mvalue || unpro_prime_imp[i].second_merge_value[3] == minterm_value[k].mvalue)
                    {
                        minterm_value[k].picked = true;
                        minterm_value[k].app_times = 0;
                        //cout << minterm_value[k].mvalue << " is picked " << minterm_value[k].app_times << " appear times is " << endl;
                    }
                } 
            }
            else
            {
                for(int j = 0; j < minterm_value.size(); j++)
                {
                    if(unpro_prime_imp[i].third_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[3] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[4] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[5] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[6] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[7] == minterm_value[j].mvalue)
                    {
                        minterm_value[j].picked = true;
                        minterm_value[j].app_times = 0;
                        //cout << minterm_value[j].mvalue << " is picked "<< minterm_value[j].app_times << " appear times is "  << endl;
                    }
                }
            }

        }
    }

    //for(int j = 0; j < minterm_value.size(); j++)
    //{
    //    if(minterm_value[j].picked == false)
    //    {
    //        cout << minterm_value[j].mvalue << " is not picked ! " << endl; 
    //    }
    //
    //}
}

vector<Imp> QM::compare_one_num(Imp a)
{
    vector<Imp> choose_vec;
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        if(unpro_prime_imp[i].first_merge_value[1] == -1)
        {
            if(unpro_prime_imp[i].mvalue == a.mvalue)
            {
                choose_vec.push_back(unpro_prime_imp[i]);
            }
             
        }
        else if(unpro_prime_imp[i].second_merge_value[1] == -1)
        {
            if(unpro_prime_imp[i].first_merge_value[0] == a.mvalue || unpro_prime_imp[i].first_merge_value[1] == a.mvalue)
            {
                choose_vec.push_back(unpro_prime_imp[i]);
            }
            
        }
        else if(unpro_prime_imp[i].third_merge_value[1] == -1)
        {
            if(unpro_prime_imp[i].second_merge_value[0] == a.mvalue || unpro_prime_imp[i].second_merge_value[1] == a.mvalue || unpro_prime_imp[i].second_merge_value[2] == a.mvalue || unpro_prime_imp[i].second_merge_value[3] == a.mvalue)
            {
                choose_vec.push_back(unpro_prime_imp[i]);
            }
             
        }
        else
        {
            if(unpro_prime_imp[i].third_merge_value[0] == a.mvalue || unpro_prime_imp[i].third_merge_value[1] == a.mvalue || unpro_prime_imp[i].third_merge_value[2] == a.mvalue || unpro_prime_imp[i].third_merge_value[3] == a.mvalue || unpro_prime_imp[i].third_merge_value[4] == a.mvalue || unpro_prime_imp[i].third_merge_value[5] == a.mvalue || unpro_prime_imp[i].third_merge_value[6] == a.mvalue || unpro_prime_imp[i].third_merge_value[7] == a.mvalue)
            {
                choose_vec.push_back(unpro_prime_imp[i]);
            }
        }
    }

    for(int i = 0; i < choose_vec.size(); i++)
    {
        if(choose_vec[i].second_merge_value[1] == -1)
        {
            choose_vec[i].Import = change2point(Get_app_times(choose_vec[i].first_merge_value[0])) + change2point(Get_app_times(choose_vec[i].first_merge_value[1])) + (choose_vec[i].weight);
            //cout << choose_vec[i].first_merge_value[0] << " , " << choose_vec[i].first_merge_value[1] << " the importance point is " << choose_vec[i].Import << endl;
        }
        else if(choose_vec[i].third_merge_value[1] == -1)
        {
            choose_vec[i].Import = change2point(Get_app_times(choose_vec[i].second_merge_value[0])) + change2point(Get_app_times(choose_vec[i].second_merge_value[1])) + change2point(Get_app_times(choose_vec[i].second_merge_value[2])) + change2point(Get_app_times(choose_vec[i].second_merge_value[3])) + (choose_vec[i].weight);
            //cout << choose_vec[i].second_merge_value[0] << " , " << choose_vec[i].second_merge_value[1] << " , " << choose_vec[i].second_merge_value[2] << " , " << choose_vec[i].second_merge_value[3] << " the importance point is " <<  choose_vec[i].Import << endl;
        }
    }

    return choose_vec;

}

Imp QM::max_Imp(vector<Imp> a)
{
    Imp temp = a[0];
    Imp max;
    for(int i = 1; i < a.size(); i++)
    {
        max = (temp.Import >= a[i].Import) ? (temp) : (a[i]);
        temp = max;
    }

    return max;
}

void QM::exclude_prime()
{
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        for(int j = 0; j < minterm_value.size(); j++)
        {
            if(unpro_prime_imp[i].prime == 0)
            { 
                if(unpro_prime_imp[i].second_merge_value[1] == -1)
                {
                    if((unpro_prime_imp[i].first_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].first_merge_value[1] == minterm_value[j].mvalue) && minterm_value[j].picked == 1)
                    {
                        unpro_prime_imp[i].prime = 2;
                    }
                }
                else if(unpro_prime_imp[i].third_merge_value[1] == -1)
                {
                    if((unpro_prime_imp[i].second_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].second_merge_value[3] == minterm_value[j].mvalue) && minterm_value[j].picked == 1 )
                    {
                        unpro_prime_imp[i].prime = 2;
                    }
                }
                else
                {
                    if((unpro_prime_imp[i].third_merge_value[0] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[1] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[2] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[3] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[4] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[5] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[6] == minterm_value[j].mvalue || unpro_prime_imp[i].third_merge_value[7] == minterm_value[j].mvalue) && minterm_value[j].picked == 1)
                    {
                        unpro_prime_imp[i].prime = 2;
                    }
                }
            }
        }
    }
}

void QM::reset()
{
    for(int i = 0; i < minterm_value.size(); i++)
    {
        minterm_value[i].picked = false;
    }

    for(int j = 0; j < minterm_value.size(); j++)
    {
        unpro_prime_imp[j].prime = 0;
    }
}

void QM::old_final_prime_imp()
{
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        if(unpro_prime_imp[i].prime == 1)
        {
            old_final_imp.push_back(unpro_prime_imp[i]); 
        }
    }

    //int literal = 0;
    //for(int j = 0; j < old_final_imp.size(); j++)
    //{
    //    for(int k = 0; k < old_final_imp[j].bin_min.size(); k++)
    //    {
    //        if(old_final_imp[j].bin_min[k] != '-')
    //        {
    //            literal = literal + 1;
    //        }
    //    }
    //}
    //for(int i = 0; i < old_final_imp.size(); i++)
    //{
    //    cout << old_final_imp[i].bin_min << endl;
    //}
    //cout << old_final_imp.size() << endl;
    //print_sort(old_final_imp);
    //cout << "literal=" << literal << endl;
}

void QM::final_prime_imp()
{
    for(int i = 0; i < unpro_prime_imp.size(); i++)
    {
        if(unpro_prime_imp[i].prime == 1)
        {
            final_imp.push_back(unpro_prime_imp[i]); 
        }
    }

    //int literal = 0;
    //for(int j = 0; j < final_imp.size(); j++)
    //{
    //    for(int k = 0; k < final_imp[j].bin_min.size(); k++)
    //    {
    //        if(final_imp[j].bin_min[k] != '-')
    //        {
    //            literal = literal + 1;
    //        }
    //    }
    //}
    //for(int i = 0; i < final_imp.size(); i++)
    //{
    //    cout << final_imp[i].bin_min << endl;
    //}
    //cout << final_imp.size() << endl;
    //print_sort(final_imp);
    //cout << "literal = " << literal << endl;
}

vector<Imp> QM::literal_compare()
{
    int old_literal = 0;
    for(int j = 0; j < old_final_imp.size(); j++)
    {
        for(int k = 0; k < old_final_imp[j].bin_min.size(); k++)
        {
            if(old_final_imp[j].bin_min[k] != '-')
            {
                old_literal = old_literal + 1;
            }
        }
    }
    
    int literal = 0;
    for(int j = 0; j < final_imp.size(); j++)
    {
        for(int k = 0; k < final_imp[j].bin_min.size(); k++)
        {
            if(final_imp[j].bin_min[k] != '-')
            {
                literal = literal + 1;
            }
        }
    }

    if(old_literal <= literal)
    {
        //cout << " old is better !" << endl;
        return old_final_imp;
    }
    else
    {
        //cout << " new is better !" << endl;
        return final_imp;
    }
}

void Output(ofstream &fout , QM &QM)
{
    
    int prime_implicants = QM.unpro_prime_imp.size();
    fout << ".p " << prime_implicants << endl;
    int count = 0;
    vector<string> outputs_prime;
    for (int i = 0; i < QM.unpro_prime_imp.size(); i++)
    {
        outputs_prime.push_back(QM.unpro_prime_imp[i].bin_min);
    }
    sort(outputs_prime.rbegin(), outputs_prime.rend());
    for (int j = 0; j < outputs_prime.size(); j++)
    {      
        if(count == 15)
        {
            break;
        }
           
        for (int i = 0; i < outputs_prime[j].size(); i++)
        {
            if (outputs_prime[j][i] == '1')
            {    
                fout << (char)('A' + i);
            }
            else if (outputs_prime[j][i] == '0')
            {
                fout << (char)('A' + i) << "'";
            }
        }
        fout << endl;
        count = count + 1;
    }

    fout << " " << endl;

    int literal = 0;
    //vector<Imp> answer_imp = QM.literal_compare();
    vector<Imp> answer_imp = (QM.dc_set_min.size() == 0 ) ? QM.old_final_imp : QM.new_final_imp;
    int minimum_cover = answer_imp.size();
    fout << ".mc " << minimum_cover << endl;
    vector<string> outputs_mini;
    for (int i = 0; i < answer_imp.size(); i++)
    {
        outputs_mini.push_back(answer_imp[i].bin_min);
    }
    sort(outputs_mini.rbegin(), outputs_mini.rend());
    for (int i = 0; i < outputs_mini.size(); i++)
    {      
        for (int j = 0; j < outputs_mini[i].size(); j++)
        {
            if (outputs_mini[i][j] == '1')
            {    
                literal = literal + 1;
                fout << (char)('A' + j);
            }
            else if (outputs_mini[i][j] == '0')
            {
                literal = literal + 1;
                fout << (char)('A' + j) << "'";
            }
        }
        fout << endl;
    }
    fout << "literal=" << literal << endl;
}

int main(int argc, char *argv[]){
    
    if( argc != 3 ) {
		cerr<<"Wrong number of arguments. Please execute as follow : \n"
			<<"./hw1.o <input_file name> <output_file name>"<< endl;
		exit(-1);
	}
	ifstream fin( argv[1] );
	ofstream fout( argv[2] );
    
    QM a;
    Init( fin , a );
    //a.getinput();
    a.or_classify();
    a.first_merge();
    if(a.num_var >= 2)
    {
        a.second_merge();
    }
    if(a.num_var == 8)
    {
        a.third_merge();
    }
    a.do_unpro_prime_imp();
    //cout << "        " << endl;
    a.appear_times();
    
    
    if(a.dc_set_min.size() != 0)
    {
        a.new_find_prime();
    }
    else if(a.dc_set_min.size() == 0)
    {
        a.old_find_prime();
        a.old_final_prime_imp();
    }
    //while(a.check_catch() == 0)
    //{
    //}
    //a.reset();

    //int k = 1;
    //while(k != 5)
    //{
    //    a.find_prime(k);
    //    k = k + 1;
    //}
    //a.final_prime_imp();

    Output( fout , a );
    return 0;
}

