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
    long long hash() const;
    list<Position> possible_next_positions();
};

int Position::max_i;
int Position::max_j;

long long Position::hash() const {
    int directions=0;
    switch (last_moves.first)
    {
        case '>':
            directions += 1;
            break;
        case 'v':
            directions +=10;
            break;
        case '<':
            directions +=100;
            break;
        case '^':
            directions +=1000;
            break;
    }
    long long ret = i;
    ret *= 100000000;
    ret += j*10000 + directions*last_moves.second;
    return ret;
}

Position reverse_hash(int hash) {
    int i = round(float(hash)/100000000);
    int j = round(float(hash-i*100000000)/10000);
    int directions=hash-i*100000000-j*10000;
    if(directions%1000){
        Position p(i,j,{'^',directions%1000});
        return p;
    } else if(directions%100){
        Position p(i,j,{'<',directions%100});
        return p;
    } else if(directions%10){
        Position p(i,j,{'v',directions%10});
        return p;
    } else if(directions>0){
        Position p(i,j,{'>',directions});
        return p;
    } else{
        Position p(i,j,{'.',1});
        return p;
    }
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

void find_min(list<Position>& queue,set<int>& seen,vector<vector<int>>& map_temp_loss,map<int,int>& memo,int & counter){
    counter++;
    if(!queue.size()){
        return;
    }
    Position position(queue.front());
    queue.pop_front();
    int hash = position.hash();
    seen.insert(position.hash());
    
    bool end = false;
    if(position.i==position.max_i-1 && position.j==position.max_j-1){
        memo[hash]=0;
        end = true;
    }
    list<Position> next_positions = position.possible_next_positions();
    list<Position> real_next_positions;
    for(Position const& p:next_positions){
        auto ret = seen.insert(p.hash());
        if(ret.second && !end){
            queue.push_back(p);
            real_next_positions.push_back(p);
        }
    }
    find_min(queue, seen,map_temp_loss,memo,counter);
    if(end){
        return;
    }
    vector<int> values;
    for(auto ite = real_next_positions.begin();ite!=real_next_positions.end();++ite){
        values.push_back(memo[ite->hash()]+map_temp_loss[ite->i][ite->j]);
    }
    auto min = std::min_element(values.begin(), values.end());
    if(min!=values.end()){
        memo[hash] =  *min;
    } else{
        memo[hash] = 99999999;
    }
    return;
}

void build_memo(list<Position>& queue,map<long long,int>& memo,vector<vector<int>>& map_temp_loss, int& counter){
    long long new_hash;
    long long hash;
    int current_loss;
    int modified = false;
    while(queue.size()){
        counter++;
        Position p(queue.front());
        queue.pop_front();
        if(!(p.i==p.max_i-1 && p.j==p.max_j-1)){
            hash = p.hash();
            list<Position> next_positions = p.possible_next_positions();
            for(Position const & new_p:next_positions){
                new_hash = new_p.hash();
                current_loss = map_temp_loss[new_p.i][new_p.j]+memo[hash];
                if(memo.find(new_hash) == memo.end() || memo[new_hash] > current_loss){
                    memo[new_hash] = current_loss;
                    queue.push_back(new_p);
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
    map<long long,int> memo;
    Position position(0,0,{'.',1});
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