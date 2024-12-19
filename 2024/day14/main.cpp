#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <set>
#include <vector>
#include <numeric>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::smatch;
using std::ifstream;
using std::pair;
using std::map;
using std::set;

struct Robot{
    Robot(int x,int y,int v_x,int v_y):position(x,y),velocity(v_x,v_y){}
    pair<int,int> position;
    pair<int,int> velocity;
    static int max_x,max_y;
    void move(int s){
        int new_x = (position.first + s * velocity.first)%max_x;
        int new_y = (position.second + s * velocity.second)%max_y;
        position.first = new_x>=0?new_x:new_x+max_x;
        position.second = new_y>=0?new_y:new_y+max_y;
    }
    int quadrant(){
        if(position.first  < max_x / 2 && position.second  < max_y / 2){
            return 1;
        }
        if(position.first  > max_x / 2 && position.second  < max_y / 2){
            return 2;
        }
        if(position.first  < max_x / 2 && position.second  > max_y / 2){
            return 3;
        }
        if(position.first  > max_x / 2 && position.second  > max_y / 2){
            return 4;
        }
        return 0;
    }
};

int Robot::max_x,Robot::max_y;

void plot(vector<Robot>& robots){
    vector<string> screen;
    std::ofstream out("output");
    for(unsigned i = 0; i < Robot::max_y;++i){
        string line(Robot::max_x,' ');
        screen.emplace_back(line);
    }
    for(const Robot& robot:robots){
        screen[robot.position.second][robot.position.first] = '*';
    }
    for(const string& line:screen){
        out << line << endl;
    }
    out.close();
}

int main(){
    //Robot::max_x = 11,Robot::max_y = 7;
    Robot::max_x = 101,Robot::max_y = 103;
    std::ifstream file("input");
    string line;
    long long ret_2 = 0;
    string pattern("p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)");
    regex r(pattern);
    smatch results;
    map<int,int> count_quadrant;
    vector<Robot> robots;
    while(getline(file,line)){
        regex_search(line, results, r);
        auto a = results.size();
        Robot robot(stoi(results.str(1)),stoi(results.str(2)),stoi(results.str(3)),stoi(results.str(4)));
        robot.move(100);
        robots.push_back(robot);
        count_quadrant[robot.quadrant()]+=1;
    }
    file.close();
    cout<< "Part 1: " << count_quadrant[1] * count_quadrant[2] * count_quadrant[3] * count_quadrant[4] << endl;   

    /// Part 2
    set<int> check_pos;
    bool no_duplicate;
    for(unsigned i = 101; i<10000;++i){
        check_pos.clear();
        no_duplicate = true;
        for(Robot& robot: robots){
            robot.move(1);
            auto ret  = check_pos.insert(robot.position.first * 1000  + robot.position.second );
            no_duplicate &= ret.second;
        }
        if(no_duplicate){
            plot(robots);
            cout<< "Part 2: " << i << endl;  
        }   
    }
    return 0;
}