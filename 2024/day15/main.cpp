#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::list;

struct Tile{
    Tile(const char c):is_wall(c=='#'),is_box_l(c=='['),is_box_r(c==']'),is_robot(c=='@'){}
    bool is_wall;
    bool is_box_l;
    bool is_box_r;
    bool is_robot;
};

struct Warehouse{
    int i;
    int j;
    vector<vector<Tile>> plan;
    list<char> instructions;
    void process();
    void plot();
    int get_gps();
};

int Warehouse::get_gps(){
    int ret=0;
    for(size_t i=0;i<plan.size();++i){
        for(size_t j=0;j<plan[0].size();++j){
            if(plan[i][j].is_box_r)
                ret += i*100+j-1;
        }
    }
    return ret;
}

void Warehouse::plot(){
    std::ofstream out("output");
    string line_str;
    for(const vector<Tile>& line:plan){
        line_str.clear();
        for(const Tile& t:line){
            if(t.is_wall)
                line_str.push_back('#');
            else if(t.is_box_l)
                line_str.push_back('[');
            else if(t.is_box_r)
                line_str.push_back(']');
            else if(t.is_robot)
                line_str.push_back('@');
            else
                line_str.push_back('.');
        }
        out << line_str << endl;
    }
    out.close();
}

bool can_move(vector<vector<Tile>>& plan,int i_incr,int i, int j){
    int new_i = i + i_incr;
    if(plan[new_i][j].is_wall)
        return false;
    else if(!plan[new_i][j].is_box_l&&!plan[new_i][j].is_box_r)
        return true;
    else if(plan[new_i][j].is_box_l){
        return can_move(plan,i_incr,new_i,j) && can_move(plan,i_incr,new_i,j+1);
    }
    else if(plan[new_i][j].is_box_r){
        return can_move(plan,i_incr,new_i,j) && can_move(plan,i_incr,new_i,j-1);
    } else{
        throw std::runtime_error("Cannot be here");
    }
    return false;
}

void move(vector<vector<Tile>>& plan,int i_incr,int i, int j){
    int new_i = i + i_incr;
    if(plan[new_i][j].is_box_l){
        move(plan,i_incr,new_i,j),move(plan,i_incr,new_i,j+1);
    }
    if(plan[new_i][j].is_box_r){
        move(plan,i_incr,new_i,j-1),move(plan,i_incr,new_i,j);
    }
    plan[new_i][j]=plan[i][j];
    plan[i][j].is_box_l = plan[i][j].is_box_r = plan[i][j].is_robot = false;
}

void Warehouse::process(){
    const char dir = instructions.front();
    instructions.pop_front();
    switch (dir)
    {
    case '<':
        for(int k = 1;k<=j;++k){
            if(plan[i][j-k].is_wall){
                break;
            }
            if(!plan[i][j-k].is_box_l&!plan[i][j-k].is_box_r){
                for(unsigned l=j-k;l<j;++l){
                    plan[i][l] = plan[i][l+1];
                }
                plan[i][j].is_robot = false;
                --j;
                break;
            }
        }
        break;
    case '>':
        for(int k = 1;k<plan[0].size()-j;++k){
            if(plan[i][j+k].is_wall){
                break;
            }
            if(!plan[i][j+k].is_box_l&!plan[i][j+k].is_box_r){
                for(unsigned l=j+k;l>j;--l){
                    plan[i][l] = plan[i][l-1];
                }
                plan[i][j].is_robot = false;
                ++j;
                break;
            }
        }
        break;
    case '^':
        if(can_move(plan,-1,i,j)){
            move(plan,-1,i,j);
            --i;
        }
        break;
    case 'v':
        if(can_move(plan,1,i,j)){
            move(plan,1,i,j);
            ++i;
        }
    }
}

int main(){
    std::ifstream file("input");
    string line;
    Warehouse warehouse;

    getline(file,line);
    int i=0,j=0;
    while(line.size()){
        vector<Tile> temp;
        j=0;
        for(const char& c:line){
            if(c=='#'){
                temp.emplace_back('#');
                temp.emplace_back('#');
            }
            if(c=='O'){
                temp.emplace_back('[');
                temp.emplace_back(']');
            }
            if(c=='.'){
                temp.emplace_back('.');
                temp.emplace_back('.');
            }
            
            if(c =='@'){
                temp.back().is_robot = true;
                warehouse.i=i;
                warehouse.j=j;
                temp.emplace_back('@');
                temp.emplace_back('.');
            }
            j+=2;
        }
        warehouse.plan.emplace_back(temp);
        ++i;
        getline(file,line);
    }
    while(getline(file,line)){
        for(const char& c:line){
            warehouse.instructions.emplace_back(c);
        }
    }
    file.close();

    warehouse.plot();
    while(warehouse.instructions.size()){
        warehouse.process();
    }
    warehouse.plot();
    cout<< "Part 2: " << warehouse.get_gps() << endl;   
    return 0;
}