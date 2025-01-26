
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::pair;
using std::map;
using std::set;
using std::find;

struct Position{
    Position(const pair<int,int> c, const pair<int,int> f, const int s):coord(c),from(f),steps(s){}
    pair<int,int> coord;
    pair<int,int> from;
    int steps;
};

struct Vertex{
    Vertex(const pair<int,int> c):coord(c){}
    pair<int,int> coord;
    list<pair<Vertex*,int>> connected_to;
};

struct Vertices{
    map<pair<int,int>,Vertex*> vertices;
    ~Vertices(){
        for(auto& [c,v]:vertices){
            delete v;
            v = nullptr;
        }
    }
};

void wfs(pair<int,int> coord,set<pair<int,int>>& seen,vector<vector<char>>& forest,Vertices& v){
    list<pair<pair<int,int>,int>> queue{{coord,0}};
    int dist = 0;
    while(queue.size()){
        const auto [head,dist] = queue.front();
        queue.pop_front();
        list<pair<int,int>> possible_next{{head.first-1,head.second},{head.first+1,head.second},{head.first,head.second-1},{head.first,head.second+1}};
        for(const auto& [i,j]:possible_next){
            if(i>=0&&i<forest.size()&&j>=0&&j<forest[0].size()&&forest[i][j]!='#'){
                const auto ret_insert = seen.insert({i,j});
                if((coord.first!=i||coord.second!=j) && v.vertices.find({i,j})!=v.vertices.end()){
                    v.vertices[coord]->connected_to.emplace_back(v.vertices[{i,j}],dist+1);
                    v.vertices[{i,j}]->connected_to.emplace_back(v.vertices[coord],dist+1);
                } else if(ret_insert.second){
                    queue.push_back({{i,j},dist+1});
                }
        
            }
        }
    }
}

int dfs_stupid(pair<int,int> coord,Vertices& v,set<pair<int,int>> seen,int dist,const pair<int,int>& coord_final){
    if(coord==coord_final){
        return dist;
    }
    int ret = -1000000;
    for(const auto [next,d]:v.vertices[coord]->connected_to){
        auto temp_set = seen;
        const auto ret_insert = temp_set.insert(next->coord);
        if(ret_insert.second){
            ret = std::max(ret,dfs_stupid(next->coord,v,temp_set,dist+d,coord_final));
        }
    }
    return ret;
}

int main(){
    /*
    the trails look  enclosed i.e. the forks are always "one way" and every path finds the exit
    so it looks like a nomal DPS would do the trick
    everytime we are at a fork (multiple path merging, we discard current until we see the last)
    to avoid redoing the same work multiple times.
    */
    std::ifstream file("input");
    string line;
    vector<vector<char>> forest;
    while(getline(file,line)){
        vector<char> temp;
        for(const char& c:line){
            temp.emplace_back(c);
        }
        forest.emplace_back(temp);
    }
    map<pair<int,int>,int> forks;
    Position pos = Position({1,1},{0,1},1);
    list<Position> queue{pos}; //start one step in to avoid checking out of bounds
    
    const pair<int,int> coord_final = {forest.size()-1,forest.size()-2};
    Vertices v;
    v.vertices[{0,1}] = new Vertex({0,1});
    v.vertices[coord_final] = new Vertex(coord_final);
    while(queue.size()){
        pos = queue.front();
        queue.pop_front();
        if(pos.coord.first==forest.size()-1&&pos.coord.second==forest.size()-2){
            break;
        }
        int incoming = 0;
        vector<pair<int,int>> next_coords{{pos.coord.first+1,pos.coord.second},{pos.coord.first-1,pos.coord.second},{pos.coord.first,pos.coord.second+1},{pos.coord.first,pos.coord.second-1}};
        for(const auto& next_coord:next_coords){
            if(forest[next_coord.first][next_coord.second]=='v' && next_coord.first<pos.coord.first)
                ++incoming;
            if(forest[next_coord.first][next_coord.second]=='^' && next_coord.first>pos.coord.first)
                ++incoming;
            if(forest[next_coord.first][next_coord.second]=='<' && next_coord.second>pos.coord.second)
                ++incoming;
            if(forest[next_coord.first][next_coord.second]=='>' && next_coord.second<pos.coord.second)
                ++incoming;
        }
        int incoming_part2 = 0;
        for(const auto& next_coord:next_coords){
            if(forest[next_coord.first][next_coord.second]!='#')
                ++incoming_part2;
        }
        if(incoming_part2>2){
            v.vertices[pos.coord] = new Vertex(pos.coord);
        }
        if(incoming < 2 || ++forks[pos.coord]==incoming){
            list<Position> queue_temp;
            for(const auto& next_coord:next_coords){
                if(!(next_coord.first == pos.from.first && next_coord.second == pos.from.second)){
                    switch (forest[next_coord.first][next_coord.second])
                    {
                    case '.':
                        queue.push_back(Position({next_coord.first,next_coord.second},{pos.coord.first,pos.coord.second},pos.steps+1));
                        break;
                    case '^':
                        if(next_coord.first<pos.coord.first)
                            queue_temp.push_back(Position({next_coord.first-1,next_coord.second},{pos.coord.first,pos.coord.second},pos.steps+2));
                        break;
                    case 'v':
                        if(next_coord.first>pos.coord.first)
                            queue_temp.push_back(Position({next_coord.first+1,next_coord.second},{pos.coord.first,pos.coord.second},pos.steps+2));
                        break;
                    case '<':
                        if(next_coord.second<pos.coord.second)
                            queue_temp.push_back(Position({next_coord.first,next_coord.second-1},{pos.coord.first,pos.coord.second},pos.steps+2));
                        break;
                    case '>':
                        if(next_coord.second>pos.coord.second)
                            queue_temp.push_back(Position({next_coord.first,next_coord.second+1},{pos.coord.first,pos.coord.second},pos.steps+2));
                        break;
                    }
                }
            }
            for(const auto& p:queue_temp) queue.push_back(p);
        }
    }
    int ret_1 = pos.steps;
    cout << "Part 1: " << ret_1 << endl;

    set<pair<int,int>> seen;
    int number_edges = 0;
    for(const auto& [c,v_ptr]:v.vertices){
        seen.insert(c);
        wfs(c,seen,forest,v);
        number_edges += v.vertices[c]->connected_to.size();
    }
    number_edges/=2;
    
    int ret_2 = dfs_stupid({0,1},v,{{0,1}},0,coord_final);
    cout << "Part 2: " << ret_2 << endl;
    return 0;
}