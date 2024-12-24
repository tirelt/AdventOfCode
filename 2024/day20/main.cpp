#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stdexcept>

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::list;
using std::vector;
using std::map;
using std::set;

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

void depth_search(list<pair<int,int>>& queue, vector<vector<Tile>>& race_track, map<pair<int,int>,int>& memo, map<pair<int,int>,int>& memo_exit,int max_depth){
    while(queue.size() && memo[queue.front()]<max_depth){
        const pair<int,int> head = queue.front();
        queue.pop_front();
        list<pair<int,int>> new_queue;
        if(head.first+1<race_track.size())
            new_queue.push_back({head.first+1,head.second});
        if(head.first-1>=0 )
            new_queue.push_back({head.first-1,head.second});
        if(head.second+1<race_track[0].size())
            new_queue.push_back({head.first,head.second+1});
        if(head.second-1>=0)
            new_queue.push_back({head.first,head.second-1});
        for(const auto& el:new_queue){
            if(memo.find(el)==memo.end()){
                memo[el] = memo[head] + 1;
                queue.push_back(el);
            }
        }
        list<pair<int,int>> exits;
            if(head.first+1<race_track.size() && !race_track[head.first+1][head.second].is_wall)
                exits.push_back({head.first+1,head.second});
            if(head.first-1>=0 && !race_track[head.first-1][head.second].is_wall)
                exits.push_back({head.first-1,head.second});
            if(head.second+1<race_track[0].size() && !race_track[head.first][head.second+1].is_wall)
                exits.push_back({head.first,head.second+1});
            if(head.second-1>=0 && !race_track[head.first][head.second-1].is_wall)
                exits.push_back({head.first,head.second-1});
            for(const auto& el:exits){
                if(memo_exit.find(el)==memo_exit.end()){
                    memo_exit[el] = memo[head] + 1;
                }
            }
    }
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
    
    //Part 1
    for(const auto& coord:path){
        if(coord.first + 2 < race_track.size() && race_track[coord.first+1][coord.second].is_wall &&  !race_track[coord.first+2][coord.second].is_wall && race_track[coord.first+2][coord.second].dist < race_track[coord.first][coord.second].dist)
            cheat_numbers[race_track[coord.first][coord.second].dist - race_track[coord.first + 2][coord.second].dist - 2] += 1;
        if(coord.first - 2 >= 0 && race_track[coord.first-1][coord.second].is_wall &&  !race_track[coord.first-2][coord.second].is_wall && race_track[coord.first-2][coord.second].dist < race_track[coord.first][coord.second].dist)
            cheat_numbers[race_track[coord.first][coord.second].dist - race_track[coord.first - 2][coord.second].dist - 2] += 1;
        if(coord.second + 2 < race_track[0].size() && race_track[coord.first][coord.second+1].is_wall &&  !race_track[coord.first][coord.second+2].is_wall && race_track[coord.first][coord.second+2].dist < race_track[coord.first][coord.second].dist)
            cheat_numbers[race_track[coord.first][coord.second].dist - race_track[coord.first][coord.second+2].dist - 2] += 1;
        if(coord.second - 2 >= 0 && race_track[coord.first][coord.second-1].is_wall &&  !race_track[coord.first][coord.second-2].is_wall && race_track[coord.first][coord.second-2].dist < race_track[coord.first][coord.second].dist)
            cheat_numbers[race_track[coord.first][coord.second].dist - race_track[coord.first ][coord.second-2].dist - 2] += 1;
    }
    int ret_1 = 0;
    for(const auto& temp:cheat_numbers){
        if(temp.first>=100)
            ret_1 += temp.second;
    }
    cout<< "Part 1: " << ret_1 << endl;

    //Part 2
    cheat_numbers.clear();
    int max_depth = 20;
    for(const auto& entry:path){
        //could do some smart memo somehwere
        map<pair<int,int>,int> memo;
        memo[entry] = 0;
        list<pair<int,int>> queue{entry};
        map<pair<int,int>,int> memo_exit;
        depth_search(queue, race_track, memo, memo_exit, max_depth);
        for(const auto& [exit,depth]:memo_exit){
            int saved = race_track[entry.first][entry.second].dist - race_track[exit.first][exit.second].dist - depth;
            if(exit.first ==  7 && exit.second == 3)
                auto v = 1;
            if(saved == 72)
                auto t= 1;
            if(saved>0){
                cheat_numbers[saved] += 1;
            }
        }
    }
    int ret_2 = 0;
    for(const auto& temp:cheat_numbers){
        if(temp.first>=100)
            ret_2 += temp.second;
    }
    cout<< "Part 2: " << ret_2 << endl;
    return 0;
}