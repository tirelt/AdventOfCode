#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <stdexcept>

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::list;
using std::vector;
using std::map;

struct Tile{
    bool is_wall;
    int dist = -1;
    bool is_end;
};

void get_dist(int i, int j, int from_i, int from_j,list<pair<int,int>>& path,vector<vector<Tile>>& race_track){
    if(race_track[i][j].is_end){
        race_track[i][j].dist = 0;
        return;
    }
    int new_i=i, new_j=j;
    if(i-1>=0 && i-1 != from_i && !race_track[i-1][j].is_wall){
        --new_i;
    }
    else if(i+1<race_track.size() && i+1 != from_i && !race_track[i+1][j].is_wall){
        ++new_i;
    }
    else if(j-1>=0 && j-1 != from_j && !race_track[i][j-1].is_wall){
        --new_j;
    } else if(j+1<race_track[0].size() && j+1 != from_j && !race_track[i][j+1].is_wall){
        ++new_j;
    }
    path.emplace_back(new_i,new_j);
    get_dist(new_i,new_j,i,j,path,race_track); 
    race_track[i][j].dist = 1+race_track[new_i][new_j].dist;
}

int main(){
    std::ifstream file("input");
    string line;
    int i=0,j=0,i_init,j_init;
    vector<vector<Tile>> race_track;
    while(getline(file,line)){
        vector<Tile> temp;
        j=0;
        for(const char& c:line){
            temp.emplace_back();
            temp.back().is_wall = c=='#';
            temp.back().is_end = c=='E';
            if(c=='S'){
                i_init = i;
                j_init = j;
            }
            ++j;
        }
        race_track.push_back(temp);
        ++i;
    }
    list<pair<int,int>> path{{i_init,j_init}};
    map<int,int> cheat_numbers;
    get_dist(i_init,j_init,i_init,j_init,path,race_track);
    for(const auto& coord:path){
        if(coord.first + 2 < race_track.size() && race_track[coord.first+1][coord.second].is_wall &&  !race_track[coord.first+2][coord.second].is_wall && race_track[coord.first+2][coord.second].dist < race_track[coord.first][coord.second].dist)
            cheat_numbers[race_track[coord.first][coord.second].dist - race_track[coord.first + 2][coord.second].dist - 2] += 1;
        if(coord.first - 2 >= 0 && race_track[coord.first-1][coord.second].is_wall &&  !race_track[coord.first-2][coord.second].is_wall && race_track[coord.first-2][coord.second].dist < race_track[coord.first][coord.second].dist)
            cheat_numbers[race_track[coord.first][coord.second].dist - race_track[coord.first - 2][coord.second].dist - 2] += 1;
        if(coord.second + 2 < race_track[0].size() && race_track[coord.first][coord.second+1].is_wall &&  !race_track[coord.first][coord.second+2].is_wall && race_track[coord.first][coord.second+2].dist < race_track[coord.first][coord.second].dist)
            cheat_numbers[race_track[coord.first][coord.second].dist - race_track[coord.first][coord.second+2].dist - 2] += 1;
        if(coord.second - 2 >= 0 && race_track[coord.first][coord.second-1].is_wall &&  !race_track[coord.first][coord.second-2].is_wall && race_track[coord.first][coord.second-2].dist < race_track[coord.first][coord.second].dist)
            cheat_numbers[race_track[coord.first][coord.second].dist - race_track[coord.first ][coord.second-2].dist - 2] += 1;
        if(cheat_numbers.find(82)!=cheat_numbers.end())
            auto a = 1;
    }
    int ret_1 = 0;
    for(const auto& temp:cheat_numbers){
        if(temp.first>=100)
            ret_1 += temp.second;
    }
    cout<< "Part 1: " << ret_1 << endl;
    cout<< "Part 2: " << 0 << endl;
    return 0;
}