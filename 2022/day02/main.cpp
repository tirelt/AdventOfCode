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
    map<char,map<char,int>> score_2= {
        {'A', {{'X', 3+0}, {'Y', 1+3}, {'Z', 2+6}}},
        {'B', {{'X', 1+0}, {'Y', 2+3}, {'Z', 3+6}}},
        {'C', {{'X', 2+0}, {'Y', 3+3}, {'Z', 1+6}}}
    };    
    string line;
    int total = 0,total_2 = 0;
    while(getline(file,line)){
        total += score.at(line[0]).at(line[2]);
        total_2 += score_2.at(line[0]).at(line[2]);
    }
    file.close();
    cout<< "Part 1: "<< total << endl; 
    cout<< "Part 2: "<< total_2 << endl; 
    return 0;
}