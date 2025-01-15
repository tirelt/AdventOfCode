
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <regex>

using std::cout;
using std::endl;
using std::string;
using std::set;
using std::vector;
using std::regex;
using std::smatch;
using std::regex_search;

struct Brick{
    Brick(const int& x_mi,const int& x_ma,const int& y_mi,const int& y_ma,const int& z_mi,const int& z_ma):x_min(x_mi),x_max(x_ma),y_min(y_mi),y_max(y_ma),z_min(z_mi),z_max(z_ma){}
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int z_min;
    int z_max;
};
bool compBrickPtr(const Brick* b, const Brick* p){
    return b->z_min<p->z_min;
}

struct Grid{
    vector<Brick* > bricks;
    void add_brick(const int x_mi,const int x_ma,const int y_mi,const int y_ma,const int z_mi,const int z_ma){
        bricks.push_back(new Brick(x_mi,x_ma,y_mi,y_ma,z_mi,z_ma));
    }
    void sort(){
        std::sort(bricks.begin(),bricks.end(),compBrickPtr);
    }
    Grid() = default;
    ~Grid(){
        for(const auto& b:bricks){
            delete b;
        }
    }
};

int main(){
    std::ifstream file("test_input");
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

    //for each brick need to check brick with smaller min_z to see if we can move it lower (i.e. smaller index in bricks). if they touch z_min = z_max +1. 
    //when it touches need to check if it touches other.
    cout << "Part 1: " << 0 << endl;
    cout << "Part 2: " << 0 << endl;

    return 0;
}