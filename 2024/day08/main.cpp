#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::min;

struct Location{
    Location(char const& v):value(v){is_antennas=v!='.';}
    char value;
    bool is_antinode=false;
    bool is_antennas=false;
};

int main(){
    std::ifstream file("input");
    string line;
    long long ret_1 = 0;
    long long ret_2 = 0;
    vector<vector<Location>> locations;
    int i=0,j=0;
    map<char,vector<pair<int,int>>> antennas;
    while(getline(file,line)){
        vector<Location> temp;
        j=0;
        for(const char& c: line){
            temp.emplace_back(c);
            if(c!='.'){
                antennas[c].emplace_back(i,j);
            }
            ++j;
        }
        locations.push_back(temp);
        ++i;
    }
    int new_i,new_j,d_i,d_j;
    for(auto p:antennas){
        for(size_t k=0;k<p.second.size();++k){
            for(size_t l=k+1;l<p.second.size();++l){
                d_i = p.second[k].first-p.second[l].first;
                d_j = p.second[k].second-p.second[l].second;
                new_i = p.second[k].first + d_i;
                new_j = p.second[k].second + d_j;
                if(new_i>=0&&new_i<locations.size()&&new_j>=0&&new_j<locations[0].size()&&!locations[new_i][new_j].is_antinode){
                    locations[new_i][new_j].is_antinode = true;
                    ++ret_1;
                }
                new_i = p.second[l].first - d_i;
                new_j = p.second[l].second - d_j;
                if(new_i>=0&&new_i<locations.size()&&new_j>=0&&new_j<locations[0].size()&&!locations[new_i][new_j].is_antinode){
                    locations[new_i][new_j].is_antinode = true;
                    ++ret_1;
                }
            }
        }
    }
    cout << "Part 1: " << ret_1 << endl; 

    for(vector<Location>& l:locations){
        for(Location& loc:l){
            loc.is_antinode=false;
        }
    }
    for(auto p:antennas){
        for(size_t k=0;k<p.second.size();++k){
            if(!locations[p.second[k].first][p.second[k].second].is_antinode){
                locations[p.second[k].first][p.second[k].second].is_antinode = true;
                ++ret_2;
            }
            for(size_t l=k+1;l<p.second.size();++l){
                d_i = p.second[k].first-p.second[l].first;
                d_j = p.second[k].second-p.second[l].second;
                new_i = p.second[k].first + d_i;
                new_j = p.second[k].second + d_j;
                while(new_i>=0&&new_i<locations.size()&&new_j>=0&&new_j<locations[0].size()){
                    if(!locations[new_i][new_j].is_antinode){
                        locations[new_i][new_j].is_antinode = true;
                        ++ret_2;
                    }
                    new_i += d_i;
                    new_j += d_j;
                }
                new_i = p.second[l].first - d_i;
                new_j = p.second[l].second - d_j;
                while(new_i>=0&&new_i<locations.size()&&new_j>=0&&new_j<locations[0].size()){
                    if(!locations[new_i][new_j].is_antinode){
                        locations[new_i][new_j].is_antinode = true;
                        ++ret_2;
                    }
                    new_i -= d_i;
                    new_j -= d_j;
                }
            }
        }
    }
    cout<< "Part 2: " << ret_2 << endl;  
    return 0;
}