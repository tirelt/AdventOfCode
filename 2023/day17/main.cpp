#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::list;
using std::string;
using std::map;
using std::set;

vector<int> process_line(string& line){
    vector<int> ret;
    for(char const& c : line){
        ret.push_back(c-'0');
    }
    return ret;
}

struct Position{
    Position(Position const& p):i(p.i),j(p.j),last_moves(p.last_moves){}
    Position(int k,int l,vector<char> v):i(k),j(l),last_moves(v){}
    int i;
    int j;
    static int max_i;
    static int max_j;
    vector<char> last_moves;
    int hash();
    vector<Position> possible_next_positions();
};

int Position::max_i;
int Position::max_j;

int Position::hash(){
    int directions=0;
    for(char const& m:last_moves){
        switch (m)
        {
            case '>':
                directions +=1;
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
    }
    return i*100000000 + j*10000 + directions;
}

vector<Position> Position::possible_next_positions(){
    list<char> possible{'>','v','<','^'};
    bool same = true;
    for(unsigned i = 0; i<last_moves.size()-1;++i){
        same = same && (last_moves[i]==last_moves[i+1]);
    }
    if(same){
        possible.remove(last_moves[0]);
    }
    vector<Position> ret;
    int new_i,new_j;
    bool add;
    for(char const& c: possible){
        vector<char>new_v{last_moves[1],last_moves[2],c};
        add = false;
        switch (c)
        {
            case '>':
                if(max_j>this->j+1 && last_moves[2] != '<'){
                    new_i = i;
                    new_j = j+1;
                    add = true;
                }
                break;
            case 'v':
                if(max_i>this->i+1 && last_moves[2] != '^'){
                    new_i = i+1;
                    new_j = j;
                    add = true;
                }
                break;
            case '<':
                if(0<=this->j-1 && last_moves[2] != '>'){
                    new_i = i;
                    new_j = j-1;
                    add = true;
                }
                break;
            case '^':
                if(0<=this->i-1 && last_moves[2] != 'v'){
                    new_i = i-1;
                    new_j = j;
                    add = true;
                }
                break;
        }
        if(add){
            Position p(new_i,new_j,new_v);
            ret.push_back(p);
        }
    }
    return ret;
}

int find_min(Position& position,vector<vector<int>>& map_temp_loss,map<int,int>& memo,int current_loss,int& loss_cap,int & counter){
    counter++;
    /*
    if(current_loss>loss_cap){
        return -1;
    }
    */
    int hash = position.hash();
    if(position.i==12&&position.j==12){
        auto a = 1;
    }
    if(memo.find(hash)!=memo.end() && current_loss > memo[hash]){
        return memo[hash];
    }
    if(position.i==position.max_i-1 && position.j==position.max_j-1){
        loss_cap = current_loss;
        memo[hash]=0;
        return 0;
    }
    //memo[hash] = -1;
    vector<Position> possible = position.possible_next_positions();
    vector<int> values;
    for(auto ite = possible.begin();ite!=possible.end();++ite){
        auto temp = find_min(*ite,map_temp_loss,memo,current_loss,loss_cap,counter);
        //if(temp>=0){
            values.push_back(map_temp_loss[ite->i][ite->j] + temp);
        //}
    }
    auto min = std::min_element(values.begin(), values.end());
    //if(min!=values.end()){
        memo[hash] = *min;
   // }
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
    Position position(0,0,{'.','.','.'});
    //Position position(11,12,{'<','v','>'});
    //auto test =position.possible_next_positions();
    int loss_cap = 200;
    int counter=0;
    int ret = find_min(position, map_temp_loss, memo,0,loss_cap,counter);
    cout << "The total load on the north support beams is " << 0 << endl;
    return 0;
}