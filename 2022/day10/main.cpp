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
    int res_1 = 0, x  = 1, cycle = 0;
    vector<int> interesting_cycles = {20, 60, 100, 140, 180, 220,260};
    int interesting_cycle_index = 0;
    vector<pair<int, int>> positions; 
    while(getline(file,line)){
        if(line.substr(0, 4) == "noop")
            ++cycle;
        else
            cycle += 2; 
        positions.push_back({cycle, x});
        if(cycle >= interesting_cycles[interesting_cycle_index] ){
            res_1 += x * interesting_cycles[interesting_cycle_index];
            ++interesting_cycle_index;
            if(interesting_cycle_index >= interesting_cycles.size()) {
                break;
            }
        }
        if(line.substr(0, 4) == "addx")
            x += std::stoi(line.substr(5));
    }
    positions.push_back({241, x});
    cout<< "Part 1: " << res_1 << endl; 

    cout<< "Part 2: "<< endl; 
    int index = 0;
    x = 1; 
    for(int i = 1; i <= 240; ++i){
        if((i-1)%40 == 0) cout << "\n";
        if( i > positions[index].first) 
            x = positions[++index].second;
        if(((i-1)%40+1) >= x && ((i-1)%40+1) <= x + 2)
            cout << "#";
        else
            cout << ".";
    }
    cout << endl;
    return 0;
}