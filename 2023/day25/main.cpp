
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::regex;
using std::smatch;
using std::sregex_iterator;

struct Component{
    Component(const string& s):name(s){}
    string name;
    vector<Component*> connected_to;
};
int main(){
    std::ifstream file("input");
    std::regex pattern(R"(\d{3})"); 
    std::smatch matches;
    string line;
    bool first = true;
    while(getline(file,line)){
        first = true;
        for(sregex_iterator it(line.begin(), line.end(), pattern), end_it;it != end_it; ++it){

            first = false;
        }
    }
    
    cout << "Part 1: " << 0 << endl;
    return 0;
}