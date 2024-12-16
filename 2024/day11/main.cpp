#include <fstream>
#include <iostream>
#include <string>
#include <map>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::to_string;
using std::pair;

map<long long,long long> next_step(map<long long,long long>& stones, long long& count){
    map<long long,long long> new_stones;
    for(const pair<long long,long long>& el:stones){
        if(!el.first){
            new_stones[1] += el.second;
        } else {
            string stone_str(to_string(el.first));
            if(!(stone_str.size()%2)){
                long long index(stone_str.size()/2);
                new_stones[stoll(stone_str.substr(0,index))] += el.second;
                new_stones[stoll(stone_str.substr(index))] += el.second;
            } else {
                new_stones[2024*el.first]+= el.second;
            }
        }
    }
    count = 0;
    for(const pair<long long,long long>& el:new_stones){
        count += el.second;
    }
    return new_stones;
}

int main(){
    std::ifstream file("input");
    string line;
    long long ret = 0;
    map<long long,long long> stones;
    while(getline(file,line,' ')){
        stones[stoll(line)] += 1;
    }
    for(unsigned i=1;i<76;++i){
        stones = next_step(stones, ret);
        if(i==25)
            cout<< "Part 1: " << ret << endl;   
    }
    cout<< "Part 2: " << ret << endl;  
    return 0;
}