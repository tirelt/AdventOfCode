#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <set>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::set;

int process(const vector<string>& lines, vector<pair<int,int>> knots) {
    set<pair<int,int>> visited;
    visited.insert({0, 0});
    for(const string& line : lines) {
        char direction = line[0];
        int steps = std::stoi(line.substr(2));
        pair<int,int>& head = knots.front();
        for(int i = 0; i < steps; ++i) {
            switch(direction) {
                case 'U': ++head.second; break;
                case 'D': --head.second; break;
                case 'L': --head.first; break;
                case 'R': ++head.first; break;
            }
            for(int j = 1; j < knots.size(); ++j) {
                pair<int,int>& tail = knots[j];
                pair<int,int>& head = knots[j-1];
                if(abs(head.first - tail.first) > 1 || abs(head.second - tail.second) > 1) {
                    if(head.first != tail.first) {
                        tail.first += (head.first > tail.first) ? 1 : -1;
                    }
                    if(head.second != tail.second) {
                        tail.second += (head.second > tail.second) ? 1 : -1;
                    }
                }
            }
            visited.insert(knots.back());
        }
    }
    return visited.size();
}

int main(){
    std::ifstream file("input");
    string line;
    vector<pair<int,int>> knots{2, pair<int,int>(0,0)};
    set<pair<int,int>> visited;
    visited.insert({0, 0}); 
    vector<string> lines;
    while(getline(file,line)){
        lines.push_back(line);
    }
    cout<< "Part 1: " << process(lines,{2, pair<int,int>(0,0)}) << endl; 
    cout<< "Part 2: " << process(lines,{10, pair<int,int>(0,0)}) << endl; 
    return 0;
}