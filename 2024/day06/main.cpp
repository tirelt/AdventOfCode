#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::set;

struct Pos{
    Pos(bool const & o,bool const & v):is_obstacle(o),has_visited(v){}
    bool has_visited;
    bool is_obstacle;
};

struct Current_Pos{
    unsigned direction;
    pair<unsigned,unsigned> coord;
    vector<vector<Pos>> positions;
    unsigned pos_visited = 1;
    set<unsigned> seen_pos;
    vector<pair<unsigned,unsigned>> positions_to_try;
    bool next_pos(bool check_loop=false);
};

bool Current_Pos::next_pos(bool check_loop){
    int i=coord.first,j=coord.second;
    switch (direction)
    {
        case 0:
            --i;
            break;
        case 1:
            ++j;
            break;
        case 2:
            ++i;
            break;
        case 3:
            --j;
            break;
    }
    if(i<0||i>=positions.size()||j<0||j>=positions[0].size()){
        coord = {i,j};
        return false;
    }
    if(positions[i][j].is_obstacle){
        direction = (direction+1)%4;
    } else{
        coord = {i,j};
        if(!positions[i][j].has_visited){
            positions[i][j].has_visited = true;
            positions_to_try.emplace_back(i,j);
            ++pos_visited;
        }
    }
    if(check_loop){
        auto ret = seen_pos.insert(i*1000000+j*1000+direction);
        if(!ret.second){
            return false;
        }
    }
    return true;
}
void process_line(vector<Pos>& temp,string& line,Current_Pos& current_pos,unsigned& line_count){
    temp.clear();
    unsigned col_count = 0;
    for(char const& c: line){
        temp.emplace_back(c=='#',false);
        if(c!='#'&&c!='.'){
            current_pos.coord = {line_count,col_count};
            if(c=='^')
                current_pos.direction = 0;
            if(c=='>')
                current_pos.direction = 1;
            if(c=='v')
                current_pos.direction = 2;
            if(c=='<')
                current_pos.direction = 3;
        }
        ++col_count;
    }
}

int main(){
    std::ifstream file("input");
    string line;
    unsigned ret_2 = 0;
    vector<Pos> temp;
    Current_Pos current_pos;
    unsigned line_count = 0;
    while(getline(file,line)){
        process_line(temp,line,current_pos,line_count);
        current_pos.positions.push_back(temp);
        ++line_count;
    }
    Current_Pos saved_initial_pos = current_pos;
    //Part 1 
    current_pos.positions[current_pos.coord.first][current_pos.coord.second].has_visited = true;
    while(current_pos.next_pos());
    cout << "Part 1: " << current_pos.pos_visited << endl; 

    //Part 2
    vector<pair<unsigned,unsigned>> positions_to_try(current_pos.positions_to_try);
    for(pair<unsigned,unsigned>& coord:positions_to_try){
        current_pos = saved_initial_pos;
        current_pos.positions[coord.first][coord.second].is_obstacle=true;
        while(current_pos.next_pos(true));
        if(current_pos.coord.first>=0&&current_pos.coord.first<current_pos.positions.size()&&current_pos.coord.second>=0&&current_pos.coord.second<current_pos.positions[0].size()){
            ++ret_2;
        }
    };
    cout<< "Part 2: " << ret_2 << endl;  
    return 0;
}