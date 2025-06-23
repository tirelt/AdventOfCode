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

int main(){
    std::ifstream file("input");
    string line;
    pair<int,int> head(0,0), tail(0,0);
    set<pair<int,int>> visited;
    visited.insert(tail); 
    while(getline(file,line)){
        char direction = line[0];
        int steps = std::stoi(line.substr(2));
        for(int i = 0; i < steps; ++i){
            switch(direction){
                case 'U': ++head.second; break;
                case 'D': --head.second; break;
                case 'L': --head.first; break;
                case 'R': ++head.first; break;
            }
            if(abs(head.first - tail.first) > 1 || abs(head.second - tail.second) > 1){
                if(head.first != tail.first) {
                    tail.first += (head.first > tail.first) ? 1 : -1;
                }
                if(head.second != tail.second) {
                    tail.second += (head.second > tail.second) ? 1 : -1;
                }
            }
            visited.insert(tail);
        }

    }
    cout<< "Part 1: " << visited.size() << endl; 
    cout<< "Part 2: " << 0 << endl; 
    return 0;
}