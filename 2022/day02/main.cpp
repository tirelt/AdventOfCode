#include <fstream>
#include <iostream>
#include <string>
#include <map>

using std::string;
using std::cout;
using std::endl;
using std::map;

int main(){
    std::ifstream file("input");
    map<char,map<char,int>> score = {
        {'A', {{'X', 1+3}, {'Y', 2+6}, {'Z', 3+0}}},
        {'B', {{'X', 1+0}, {'Y', 2+3}, {'Z', 3+6}}},
        {'C', {{'X', 1+6}, {'Y', 2+0}, {'Z', 3+3}}}
    };    
    string line;
    int total = 0;
    while(getline(file,line)){
        total += score.at(line[0]).at(line[2]);
    }
    file.close();
    cout<< "Part 1: "<< total << endl; 
    cout<< "Part 2: "<< 0 << endl; 
    return 0;
}