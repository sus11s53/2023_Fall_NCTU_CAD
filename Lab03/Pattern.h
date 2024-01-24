#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Pattern
{
private:
    vector<string> pins;

public:
    vector<vector<bool>> data;
    unordered_map<string, int> indices;
    Pattern(const string);
    void print();
};