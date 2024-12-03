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
    static int max_one_direction;
    static int min_one_direction;
    pair<char,unsigned> last_moves;
    long long hash() const;
    list<Position> possible_next_positions();
};

int Position::max_i;
int Position::max_j;
int Position::max_one_direction;
int Position::min_one_direction;

long long Position::hash() const {
    int directions=0;
    switch (last_moves.first)
    {
        case '>':
            directions += 100;
            break;
        case 'v':
            directions += 200;
            break;
        case '<':
            directions += 300;
            break;
        case '^':
            directions += 400;
            break;
    }
    long long ret = i*1000000;
    ret += j*1000; 
    ret += directions + last_moves.second;
    return ret;
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
    int shift;
    if(last_moves.second<min_one_direction){
        possible = {last_moves.first};
    }
    for(char const& c: possible){
        unsigned number;
        if(c==last_moves.first){
            number = last_moves.second + 1;
        } else{
            number = 1;
        }
        if(number<= max_one_direction && c != opposite_direction(last_moves.first)){
            pair<char,unsigned> new_v{c,number};
            shift = c==last_moves.first?1:min_one_direction;
            switch (c)
            {
                case '>':
                    if(max_j>j+shift){
                        ret.emplace_back(i,j+1,new_v);
                    }
                    break;
                case 'v':
                    if(max_i>i+shift){
                        ret.emplace_back(i+1,j,new_v);
                    }
                    break;
                case '<':
                    if(0<=j-shift){
                        ret.emplace_back(i,j-1,new_v);
                    }
                    break;
                case '^':
                    if(0<=i-shift){
                        ret.emplace_back(i-1,j,new_v);
                    }
                    break;
            }
        }
    }
    return ret;
}

void build_memo(list<Position>& queue,map<long long,int>& memo,vector<vector<int>>& map_temp_loss, int& counter){
    long long new_hash;
    long long hash;
    int current_loss;
    int max_value = 999999;
    bool skip = false;
    while(queue.size()){
        counter++;
        skip = false;
        Position p(queue.front());
        queue.pop_front();
        if(memo.find(new_hash) != memo.end() && memo[new_hash]>=max_value){
            skip = true;
        }
        if(!(p.i==p.max_i-1 && p.j==p.max_j-1)&&!skip){
            hash = p.hash();
            list<Position> next_positions = p.possible_next_positions();
            for(Position const & new_p:next_positions){
                current_loss = map_temp_loss[new_p.i][new_p.j]+memo[hash];
                if(new_p.i==new_p.max_i-1 && new_p.j==new_p.max_j-1){
                    max_value = std::min(max_value,current_loss);
                }
                new_hash = new_p.hash();
                if(memo.find(new_hash) == memo.end() || memo[new_hash] > current_loss){
                    memo[new_hash] = current_loss;
                    if(current_loss<max_value){
                        queue.push_back(new_p);
                    }
                }
            }
        }
    }
    return;
}
int main(){
    std::ifstream file("input");
    string line;
    vector<vector<int>> map_temp_loss;
    while(getline(file,line)){
        map_temp_loss.push_back(process_line(line));
    }
    file.close();
    Position::max_i = map_temp_loss.size();
    Position::max_j = map_temp_loss[0].size();

    //part 1: (1,3)
    //part 2: (4,10)
    Position::min_one_direction = 1;
    Position::max_one_direction = 3; 
    
    map<long long,int> memo;
    Position position(0,0,{'.',Position::min_one_direction});
    Position last(position.max_i-1,position.max_j-1,{'.',1});
    long long last_hash(last.hash());
    int counter=0;
    list<Position> queue{position};
    build_memo(queue,memo,map_temp_loss,counter);
    int ret = 999999;
    
    for(auto pair:memo){
        if(pair.first>=last_hash){
            ret = std::min(ret,pair.second);
        }
    }
    cout << "The least heat loss it can incur is " << ret << endl;
    return 0;
}