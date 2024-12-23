#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <list>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::list;
using std::set;
using std::map;
using std::priority_queue;

struct Position{
    int x;
    int y;
    int dir;
    int cum_score;

    int hash() const{
        return dir*1'000'000+x*1'000+y;
    }
    list<Position> next_position(const vector<vector<char>>& maze) const {
        list<Position> next;
        int new_x=x,new_y=y;
        switch (dir)
        {
        case 0: //up
            new_x -=1;
            break;
        case 1: //up
            new_y += 1;
            break;
        case 2: //up
            new_x +=1;
            break;
        case 3: //up
            new_y -=1;
            break;
        }
        if(new_x>=0 && new_x <= maze.size() && new_y>=0 && new_y <= maze[0].size() && maze[new_x][new_y]!='#'){
            Position temp = *this;
            temp.x = new_x;
            temp.y = new_y;
            temp.cum_score += 1;
            next.push_back(temp);
        }
        Position temp = *this;
        temp.cum_score += 1000;
        temp.dir = (temp.dir + 1 )%4;
        next.push_back(temp);
        temp.dir = (temp.dir + 2 )%4;
        next.push_back(temp);
        return next;
    }
    bool operator<(const Position& p) const{
        return p.cum_score < cum_score;
    }
};

struct Shortest{
    Shortest(){};
    Shortest(const int c,list<int> l):cum_score(c),come_from(l){}
    int cum_score;
    list<int> come_from;
};

void dijkstra(priority_queue<Position>& queue,const vector<vector<char>>& maze,map<int,Shortest>& shortest_known){
    const Position p = queue.top();
    queue.pop();
    list<Position> new_queue = p.next_position(maze);
    for(const auto& new_p:new_queue){
        int new_hash = new_p.hash();
        if(shortest_known.find(new_hash)==shortest_known.end() || shortest_known[new_hash].cum_score > new_p.cum_score){
            shortest_known[new_hash] = Shortest(new_p.cum_score,{p.hash()});
            queue.push(new_p);       
        } else if(shortest_known[new_hash].cum_score == new_p.cum_score){
            shortest_known[new_hash].come_from.push_back(p.hash());
        }
    }
}

int main(){
    std::ifstream file("input");
    string line;
    vector<vector<char>> maze;
    int i=0,j=0,x_final,y_final;
    priority_queue<Position> queue;
    map<int,Shortest> shortest_known;
    while(getline(file,line)){
        vector<char> temp;
        j = 0;
        for(const char& c:line){
            temp.emplace_back(c);
            if(c=='S'){
                Position p_init;
                p_init.x = i;
                p_init.y = j;
                p_init.dir = 1;
                p_init.cum_score = 0;
                queue.push(p_init);
                shortest_known[p_init.hash()] = Shortest(p_init.cum_score,{});
            }
            if(c=='E'){
                x_final = i;
                y_final = j;
            }
            ++j;
        }
        maze.emplace_back(temp);
        ++i;
    }
    file.close();
    
    while(queue.size() && !(queue.top().x == x_final && queue.top().y == y_final )){
        dijkstra(queue,maze,shortest_known);
    }
    cout<< "Part 1: " << queue.top().cum_score << endl;
    list<int> to_check{queue.top().hash()};
    set<int> unique_pos;
    while(to_check.size()){
        for(const int& next:shortest_known[to_check.front()].come_from){
            unique_pos.insert(next%1'000'000);
            to_check.push_back(next);
        }
        to_check.pop_front();
    }
    cout<< "Part 2: " << unique_pos.size()+1 << endl;
    return 0;
}