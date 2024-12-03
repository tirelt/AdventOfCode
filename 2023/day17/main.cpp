#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <utility>
#include <limits>

using std::cout;
using std::endl;
using std::vector;
using std::list;
using std::string;
using std::map;
using std::set;
using std::pair;

vector<int> process_line(string& line){
    vector<int> ret;
    for(char const& c : line){
        ret.push_back(c-'0');
    }
    return ret;
}

struct Position{
    Position(Position const& p):i(p.i),j(p.j),last_moves(p.last_moves){}
    Position(int k,int l,pair<char,unsigned> v):i(k),j(l),last_moves(v){}
    int i;
    int j;
    static int max_i;
    static int max_j;
    pair<char,unsigned> last_moves;
    int hash();
    list<Position> possible_next_positions();
};

int Position::max_i;
int Position::max_j;

int Position::hash(){
    int directions;
    switch (last_moves.first)
    {
        case '>':
            directions = 1;
            break;
        case 'v':
            directions =10;
            break;
        case '<':
            directions =100;
            break;
        case '^':
            directions =1000;
            break;
    }
    return i*100000000 + j*10000 + directions*last_moves.second;
}

char opposite_direction(char const& c){
    switch (c)
    {
        case '>':
            return '<';
        case 'v':
            return '^';
        case '<':
            return '>';
        case '^':
            return 'v';
    }
    return '.';
}
list<Position> Position::possible_next_positions(){
    vector<char> possible{'>','v','<','^'};
    list<Position> ret;
    int new_i,new_j;
    bool add;
    for(char const& c: possible){
        unsigned number;
        if(c==last_moves.first){
            number = last_moves.second + 1;
        } else{
            number = 1;
        }
        if(number<=3 && c != opposite_direction(last_moves.first)){
            pair<char,unsigned> new_v{c,number};
            add = false;
            switch (c)
            {
                case '>':
                    if(max_j>this->j+1){
                        ret.emplace_back(i,j+1,new_v);
                    }
                    break;
                case 'v':
                    if(max_i>this->i+1){
                        ret.emplace_back(i+1,j,new_v);
                    }
                    break;
                case '<':
                    if(0<=this->j-1){
                        ret.emplace_back(i,j-1,new_v);
                    }
                    break;
                case '^':
                    if(0<=this->i-1){
                        ret.emplace_back(i-1,j,new_v);
                    }
                    break;
            }
        }
    }
    return ret;
}

int find_min(Position& position,list<Position> queue,vector<vector<int>>& map_temp_loss,map<int,int>& memo,int & counter,list<int> stack){
    counter++;
    int hash = position.hash();
    stack.push_back(hash);
    if(position.i==12&&position.j==12){
        auto a = 1;
    }
    if(memo.find(hash)!=memo.end()){
        return memo[hash];
    }
    if(position.i==position.max_i-1 && position.j==position.max_j-1){
        memo[hash]=0;
        return 0;
    }
    list<Position> next_positions = position.possible_next_positions();
    queue.splice(queue.end(),next_positions);
    for(auto ite = queue.begin();ite!=queue.end();++ite){
        auto temp = find_min(*ite,queue, map_temp_loss,memo,counter,stack);
    }
    vector<int> values;
    for(auto ite = next_positions.begin();ite!=next_positions.end();++ite){
        values.push_back(memo[hash]+map_temp_loss[ite->i][ite->j]);
    }
    auto min = std::min_element(values.begin(), values.end());
    if(min!=values.end()){
        memo[hash] =  *min;
    } else{
        memo[hash] = 99999999;
    }
    return memo[hash];
}

int main(){
    std::ifstream file("test_input");
    string line;
    vector<vector<int>> map_temp_loss;
    while(getline(file,line)){
        map_temp_loss.push_back(process_line(line));
    }
    file.close();
    Position::max_i = map_temp_loss.size();
    Position::max_j = map_temp_loss[0].size();
    map<int,int> memo;
    set<int> has_visited;
    list<int> stack;
    //Position position(0,1,{'>','^','<'});
    //Position position(11,12,{'<','v','>'});
    //auto test =position.possible_next_positions(has_visited);
    Position position(0,0,{'.',1});
    int loss_cap = 200;
    int counter=0;
    list<Position> queue;
    int ret = find_min(position, queue, map_temp_loss, memo,counter,stack);
    cout << "The total load on the north support beams is " << 0 << endl;
    return 0;
}