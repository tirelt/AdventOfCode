
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::pair;
using std::map;

struct Position{
    Position(const pair<int,int> c, const pair<int,int> f, const int s):coord(c),from(f),steps(s){}
    pair<int,int> coord;
    pair<int,int> from;
    int steps;
};

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
        if(incoming>1){
            auto a = 1;
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
    cout << "Part 2: " << 0 << endl;

    return 0;
}