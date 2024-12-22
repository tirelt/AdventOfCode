#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stdexcept>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::list;
using std::map;
using std::set;
using std::pair;

struct Position{
    //Position(int a, int b, int d):x(a),y(b),dir(d){}
    int x;
    int y;
    int dir;
    int hash() const{
        return dir*1'000'000+x*1'000+y;
    }
    list<pair<Position,int>> next_position(const vector<vector<char>>& maze,int cum_score = 0) const {
        list<pair<Position,int>> list_next;
        Position temp = *this;
        switch (dir)
        {
        case 0: //up
            if(x-1>=0 && maze[x-1][y]!='#')
                temp.x-=1;
            break;
        case 1: //up
            if(y+1<maze[0].size() && maze[x][y+1]!='#')
                temp.y+=1;
            break;
        case 2: //up
            if(x+1<maze.size() && maze[x+1][y]!='#')
                temp.x+=1;
            break;
        case 3: //up
            if(y-1>=0 && maze[x][y-1]!='#')
                temp.y-=1;
            break;
        }
        list_next.emplace_back(temp,cum_score+1);
        temp = *this;
        temp.dir = (temp.dir + 1)%4;
        list_next.emplace_back(temp,cum_score+1000);
        temp.dir = (temp.dir + 2)%4;
        list_next.emplace_back(temp,cum_score+1000);
        return list_next;
    }
};

void build_memo(const Position& p_init, const vector<vector<char>>& maze,map<int,int>& memo){
    list<Position> queue{p_init};
    int hash = p_init.hash();
    memo[hash] = 0;
    set<int> is_in_queue{hash};
    while(queue.size())
    {
        const Position& p = queue.front();
        queue.pop_front();
        hash = p.hash();
        is_in_queue.erase(hash);
        list<pair<Position,int>> new_queue = p.next_position(maze,memo[hash]);
        for(const auto& [new_p,new_cum_score]:new_queue){
            int new_hash = new_p.hash();
            if(memo.find(new_hash)==memo.end() || memo[new_hash] > new_cum_score){
                memo[new_hash] = new_cum_score;
                if(auto ret = is_in_queue.insert(new_hash);ret.second){
                    queue.push_back(new_p);
                }
            }
        }  
    }   
}
int main(){
    std::ifstream file("input");
    string line;
    vector<vector<char>> maze;
    int i=0,j=0,x,y;
    Position p,p_final;
    map<int,int> memo;
    while(getline(file,line)){
        vector<char> temp;
        j = 0;
        for(const char& c:line){
            temp.emplace_back(c);
            if(c=='S'){
                p.x = i;
                p.y = j;
                p.dir = 1;
            }
            if(c=='E'){
                p_final.x = i;
                p_final.y = j;
            }
            ++j;
        }
        maze.emplace_back(temp);
        ++i;
    }
    file.close();
    set<int> queue;
    build_memo(p,maze,memo);
    int ret_1 = 99999999;
    for(int i=0;i<4;++i){
        p_final.dir = i;
        ret_1 = std::min(ret_1,memo[p_final.hash()]);
    }
    cout<< "Part 1: " << ret_1 << endl;
    return 0;
}