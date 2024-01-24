#include <fstream>
#include <cctype>
#include <iostream>

#include "Pattern.h"

using namespace std;

Pattern::Pattern(const string filePath)
{
    pins.clear();
    indices.clear();
    data.clear();
    ifstream patternFile(filePath);
    string line;
    while (getline(patternFile, line))
        if (line.find("input") != string::npos)
        {
            line.push_back(' ');
            for (int begin = 0, end = 0, index = 0; end < line.length(); ++end)
                if (!isalnum(line[end]))
                {
                    if (begin < end)
                    {
                        string pin = line.substr(begin, end - begin);
                        if (pin != "input")
                        {
                            pins.emplace_back(pin);
                            indices[pin] = index++;
                        }
                    }
                    begin = end + 1;
                }
        }
        else if (line.find(".end") != string::npos)
            break;
        else if (line.find("0") == string::npos && line.find("1") == string::npos)
            continue;
        else
        {
            vector<bool> digits{};
            for (const char c : line)
                if (c == '0')
                    digits.emplace_back(false);
                else if (c == '1')
                    digits.emplace_back(true);
            if (digits.size() != pins.size())
            {
                cout << "pattern format error" << endl;
                exit(-1);
            }
            data.emplace_back(digits);
        }
}
void Pattern::print()
{
    for (const vector<bool> digits : data)
    {
        for (bool digit : digits)
            cout << (digit ? "1" : "0");
        cout << endl;
    }
}