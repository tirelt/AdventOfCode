
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <list>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::vector;
using std::list;
using std::regex;
using std::smatch;
using std::regex_search;

struct Brick{
    Brick(const int& x_mi,const int& x_ma,const int& y_mi,const int& y_ma,const int& z_mi,const int& z_ma,const int& l):x_min(x_mi),x_max(x_ma),y_min(y_mi),y_max(y_ma),z_min(z_mi),z_max(z_ma),label(l){}
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int z_min;
    int z_max;
    int label;
    vector<Brick* > supported_by;

};

bool compBrickPtr(const Brick* b, const Brick* p){
    return b->z_min<p->z_min;
}

struct Grid{
    vector<Brick* > bricks;
    static int label;
    void add_brick(const int x_mi,const int x_ma,const int y_mi,const int y_ma,const int z_mi,const int z_ma){
        bricks.push_back(new Brick(x_mi,x_ma,y_mi,y_ma,z_mi,z_ma,label));
        ++label;
    }
    void sort(){
        std::sort(bricks.begin(),bricks.end(),compBrickPtr);
    }
    int size(){
        return bricks.size();
    }
    Brick*& operator[](const int i){
        return bricks[i];
    }

    Grid() = default;
    ~Grid(){
        for(const auto& b:bricks){
            delete b;
        }
    }
};

int Grid::label = 0;

int main(){
    std::ifstream file("input");
    std::regex pattern(R"((\d+),(\d+),(\d+)~(\d+),(\d+),(\d+))"); 
    std::smatch matches;
    string line;
    Grid grid;
    while(getline(file,line)){
        if(regex_search(line,matches,pattern)){
            grid.add_brick(stoi(matches.str(1)),stoi(matches.str(4)),stoi(matches.str(2)),stoi(matches.str(5)),stoi(matches.str(3)),stoi(matches.str(6)));
        }
    }
    grid.sort();
    int shift = 0;
    for(int i=0;i<grid.size();++i){
        if(grid[i]->z_min>1){
            map<int,list<Brick*>> height_collision;
            for(int j=i-1;j>=0;--j){
                if(!(grid[j]->x_min > grid[i]->x_max || grid[j]->x_max < grid[i]->x_min || grid[j]->y_min > grid[i]->y_max || grid[j]->y_max < grid[i]->y_min )){
                    height_collision[grid[j]->z_max].push_back(grid[j]);
                }
            }
            if(height_collision.size()){
                const auto& [z_collision,supported_by] = *height_collision.rbegin();
                for(const auto& b:supported_by){
                    grid[i]->supported_by.push_back(b);
                }
                shift = grid[i]->z_min - z_collision - 1;
            } else{
                shift = grid[i]->z_min - 1;
            }
            grid[i] -> z_min -= shift;
            grid[i] -> z_max -= shift; 
        }
    }
    map<int,bool> safe_brick;
    for(int i=0;i<grid.size();++i){
        safe_brick[grid[i]->label] = true;
    }
    for(int i=0;i<grid.size();++i){
        if(grid[i]->supported_by.size()==1){
            safe_brick[grid[i]->supported_by[0]->label] = false;
        }
    }
    int ret_1=0;
    for(const auto [l,safe]:safe_brick){
        if(safe)
            ++ret_1;
    }
    grid.sort();
    //for each brick need to check brick with smaller min_z to see if we can move it lower (i.e. smaller index in bricks). if they touch z_min = z_max +1. 
    //when it touches need to check if it touches other.
    cout << "Part 1: " << ret_1 << endl;
    cout << "Part 2: " << 0 << endl;

    vector<vector<vector<int >>> plan(10,vector<vector<int>>(10,vector<int>(100,0)));
    for(int i=0;i<grid.size();++i){
        for(int x = grid[i]->x_min;x<=grid[i]->x_max;++x){
            for(int y = grid[i]->y_min;y<=grid[i]->y_max;++y){
                for(int z = grid[i]->z_min;z<=grid[i]->z_max;++z){
                    if(plan[x][y][z]!=0)
                        auto a = 1; //collision ??  845 into 444
                    else
                        plan[x][y][z] = grid[i]->label;
                }
            }
        }
    }
    return 0;
}