#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::list;

struct Tile{
    Tile(const char c):is_wall(c=='#'),is_box(c=='O'),is_robot(c=='@'){}
    bool is_wall;
    bool is_box;
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
            if(plan[i][j].is_box)
                ret += i*100+j;
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
            else if(t.is_box)
                line_str.push_back('O');
            else if(t.is_robot)
                line_str.push_back('@');
            else
                line_str.push_back('.');
        }
        out << line_str << endl;
    }
    out.close();
}

void Warehouse::process(){
    const char dir = instructions.front();
    instructions.pop_front();
    bool can_move=false;
    int empty_i(i),empty_j(j), new_i(i),new_j(j);
    switch (dir)
    {
    case '<':
        for(int k = 1;k<=j;++k){
            if(plan[i][j-k].is_wall){
                break;
            }
            if(!plan[i][j-k].is_box){
                can_move = true;
                empty_j = j-k;
                --new_j;
                break;
            }
        }
        break;
    case '^':
        for(int k = 1;k<=i;++k){
            if(plan[i-k][j].is_wall){
                break;
            }
            if(!plan[i-k][j].is_box){
                can_move = true;
                empty_i = i-k;
                --new_i;
                break;
            }
        }
        break;
    case '>':
        for(int k = 1;k<plan[0].size()-j;++k){
            if(plan[i][j+k].is_wall){
                break;
            }
            if(!plan[i][j+k].is_box){
                can_move = true;
                empty_j = j+k;
                ++new_j;
                break;
            }
        }
        break;
    case 'v':
        for(int k = 1;k<plan.size()-i;++k){
            if(plan[i+k][j].is_wall){
                break;
            }
            if(!plan[i+k][j].is_box){
                can_move = true;
                empty_i = i+k;
                ++new_i;
                break;
            }
        }
    }
    if(can_move){
        plan[empty_i][empty_j].is_box = plan[new_i][new_j].is_box;
        plan[i][j].is_robot = false;
        plan[new_i][new_j].is_robot = true;
        plan[new_i][new_j].is_box = false;
        i = new_i;
        j = new_j;
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
            temp.emplace_back(c);
            if(c =='@'){
                temp.back().is_robot = true;
                warehouse.i=i;
                warehouse.j=j;
            }
            ++j;
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

    while(warehouse.instructions.size()){
        warehouse.process();
    }
    warehouse.plot();
    cout<< "Part 1: " << warehouse.get_gps() << endl;   
    cout<< "Part 2: " << 0 << endl;  
    return 0;
}