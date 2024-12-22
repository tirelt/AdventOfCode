#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <list>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::list;
using std::set;

struct Position{
    int x;
    int y;
    int dir;
    int cum_score;
    set<int> stack;

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
            //temp.stack.insert(new_x*1'000+new_y);
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
};

void dijkstra(list<Position>& queue,const vector<vector<char>>& maze,set<int>& treated){
    const Position p = queue.front();
    queue.pop_front();
    list<Position> new_queue = p.next_position(maze);
    treated.insert(p.hash());
    for(const auto& new_p:new_queue){
        if(treated.find(new_p.hash())==treated.end()){
            auto ite = queue.begin();
            bool need_to_insert=true;
            while(ite!=queue.end()){
                if(ite->hash()==new_p.hash()){
                    if( ite->cum_score > new_p.cum_score){
                        ite = queue.erase(ite);
                    } else{
                        need_to_insert = false;
                        /*
                        if (ite->cum_score = new_p.cum_score){
                            for(const int& h:new_p.stack){
                                ite->stack.insert(h);
                            }
                        }*/
                    }
                    break;
                }
                ++ite;
            }
            if(need_to_insert){
                auto ite = queue.begin();
                while(ite!=queue.end() && ite->cum_score < new_p.cum_score){
                    ++ite;
                }
                queue.insert(ite,new_p);
            }
        }
    }
}
int main(){
    std::ifstream file("input");
    string line;
    vector<vector<char>> maze;
    int i=0,j=0,x_final,y_final;
    list<Position> queue;
    Position p_init;
    while(getline(file,line)){
        vector<char> temp;
        j = 0;
        for(const char& c:line){
            temp.emplace_back(c);
            if(c=='S'){
                p_init.x = i;
                p_init.y = j;
                p_init.dir = 1;
                p_init.cum_score = 0;
                //p_init.stack.insert(p_init.x*1'000+p_init.y);
                queue.push_back(p_init);
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
    set<int> treated;
    while(queue.size() && !(queue.front().x == x_final && queue.front().y == y_final )){
        dijkstra(queue,maze,treated);
    }
    
    cout<< "Part 1: " << queue.front().cum_score << endl;
    cout<< "Part 2: " << queue.front().stack.size() << endl;
    return 0;
}