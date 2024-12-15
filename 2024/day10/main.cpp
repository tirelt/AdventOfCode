#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <list>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;
using std::list;

set<unsigned> part_1(int const i,int const j,vector<vector<int>> const& topo_map, map<unsigned,set<unsigned>>& memo){
    unsigned hash = i*100+j;
    if(memo.find(hash)!=memo.end()){
        return memo[hash];
    }
    if(topo_map[i][j]==9){
        memo[hash]={hash};
        return memo[hash];
    }
    set<unsigned> ret;
    set<unsigned> temp;
    if(i-1>=0 && (topo_map[i-1][j] - topo_map[i][j])==1){
        temp = part_1(i-1,j,topo_map,memo);
        ret.insert(temp.begin(),temp.end());
    }
    if(i+1<topo_map.size()&& (topo_map[i+1][j] - topo_map[i][j])==1){
        temp = part_1(i+1,j,topo_map,memo);
        ret.insert(temp.begin(),temp.end());
    } 
    if(j-1>=0&& (topo_map[i][j-1] - topo_map[i][j])==1){
        temp = part_1(i,j-1,topo_map,memo);
        ret.insert(temp.begin(),temp.end());
    }
    if(j+1<topo_map[0].size()&& (topo_map[i][j+1] - topo_map[i][j])==1){
        temp = part_1(i,j+1,topo_map,memo);
        ret.insert(temp.begin(),temp.end());
    }
    memo[hash] = ret;
    return ret;
}

unsigned part_2(int const i,int const j,vector<vector<int>> const& topo_map, map<unsigned,unsigned>& memo){
    unsigned hash = i*100+j;
    if(memo.find(hash)!=memo.end()){
        return memo[hash];
    }
    if(topo_map[i][j]==9){
        memo[hash] = 1;
        return memo[hash];
    }
    unsigned ret = 0;
    if(i-1>=0 && (topo_map[i-1][j] - topo_map[i][j])==1){
        ret += part_2(i-1,j,topo_map,memo);
    }
    if(i+1<topo_map.size()&& (topo_map[i+1][j] - topo_map[i][j])==1){
        ret += part_2(i+1,j,topo_map,memo);
    } 
    if(j-1>=0&& (topo_map[i][j-1] - topo_map[i][j])==1){
        ret += part_2(i,j-1,topo_map,memo);
    }
    if(j+1<topo_map[0].size()&& (topo_map[i][j+1] - topo_map[i][j])==1){
        ret += part_2(i,j+1,topo_map,memo);
    }
    memo[hash] = ret;
    return ret;
}
int main(){
    std::ifstream file("input");
    string line;
    long long ret_1 = 0;
    long long ret_2 = 0;
    vector<vector<int>> topo_map;
    map<unsigned,set<unsigned>> memo;
    unsigned i=0,j=0;
    list<pair<unsigned,unsigned>> starts;
    while(getline(file,line)){
        vector<int> temp;
        j=0;
        for(const char& c:line){
            if(c!='.'){
                temp.push_back(c-'0');
                if( c=='0'){
                    starts.emplace_back(i,j);
                }
            } else{
                temp.push_back(-1);
            }
            ++j;
        }
        topo_map.emplace_back(temp);
        ++i;
    }
    for(const pair<unsigned,unsigned> p:starts){
        ret_1 += part_1(p.first,p.second,topo_map,memo).size();
    }
    cout << "Part 1: " << ret_1 << endl; 

    map<unsigned,unsigned> memo_2;
    for(const pair<unsigned,unsigned> p:starts){
        ret_2 += part_2(p.first,p.second,topo_map,memo_2);
    }
    cout<< "Part 2: " << ret_2 << endl;  
    return 0;
}