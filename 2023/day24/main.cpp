
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::regex;
using std::smatch;
using std::regex_search;

struct Hailstone{
    Hailstone(long long pa,long long pb,long long pc,int va, int vb, int vc):px(pa),py(pb),pz(pc),vx(va),vy(vb),vz(vc){}
    long long px,py,pz;
    int vx,vy,vz;
};
int main(){
    std::ifstream file("input");
    std::regex pattern(R"((-?\d+),\s+(-?\d+),\s+(-?\d+)\s+@\s+(-?\d+),\s+(-?\d+),\s+(-?\d+))"); 
    std::smatch matches;

    string line;
    vector<Hailstone> hailstones;
    while(getline(file,line)){
        if(regex_search(line,matches,pattern)){
            hailstones.emplace_back(stoll(matches.str(1)),stoll(matches.str(2)),stoll(matches.str(3)),stoll(matches.str(4)),stoll(matches.str(5)),stoll(matches.str(6)));
        }
    }
    int ret_1 = 0;
    long long x_min = 200000000000000,x_max = 400000000000000;
    //long long x_min = 7,x_max = 27;
    for(int i=0;i<hailstones.size();++i){
        for(int j=i+1;j<hailstones.size();++j){
            int a = -hailstones[i].vx,b = hailstones[j].vx,c = -hailstones[i].vy,d = hailstones[j].vy;
            int det = a*d-b*c;
            if(det){
                long long x = hailstones[i].px - hailstones[j].px,y = hailstones[i].py - hailstones[j].py;
                float t1 = (d*x-b*y)/float(det);
                float t2 = (-c*x+a*y)/float(det);
                if(t1>0&&t2>0){
                    float x_intersect = hailstones[i].px + hailstones[i].vx*t1;
                    float y_intersect = hailstones[i].py + hailstones[i].vy*t1;
                    if(x_intersect>=x_min&&x_intersect<=x_max&&y_intersect>=x_min&&y_intersect<=x_max){
                        ++ret_1;
                    }
                }
            }
        }    
    }
    cout << "Part 1: " << ret_1 << endl;
    cout << "Part 2: " << 0 << endl;
    return 0;
}