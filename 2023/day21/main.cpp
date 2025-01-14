
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::set;
using std::list;
using std::vector;

int main(){
    /*
    Intuition, Depth first search
    after N steps he can reach all the positions (i,j) such as 
    i + j have the same parity than i_0 + j_0
    */
    std::ifstream file("input");
    string line;
    vector<vector<char>> garden;
    int i_0,j_0,i=0,j=0;
    while(getline(file,line)){
        vector<char> temp;
        j=0;
        for(auto& c:line){
            temp.emplace_back(c);
            if(c=='S'){
                i_0 = i,j_0=j;
                temp.back() = '.';
            }
            ++j;
        }
        garden.push_back(temp);
        ++i;
    }
    list<pair<pair<int,int>,int>> queue{{{i_0,j_0},0}};
    set<pair<int,int>> visited_even{{i_0,j_0}};
    set<pair<int,int>> visited_odd;
    int max_depth = 64;
    int depth_part1 = 64;
    int ret_1 = 0;
    bool locked = false;

    int depth_65 = 65;
    int ret_65_steps = 0;
    bool locked_65 = false;

    while(queue.size()){
        if(!locked && queue.front().second==depth_part1){
            ret_1 = depth_part1%2?visited_odd.size():visited_even.size();
            locked = true;
        }
        if(!locked_65 && queue.front().second==depth_65){
            ret_65_steps = depth_65%2?visited_odd.size():visited_even.size();
            locked_65 = true;
        }
        const auto [coord,d] = queue.front();
        const auto [k,l] = coord;
        queue.pop_front();
        list<pair<int,int>> to_try{{k,l+1},{k,l-1},{k+1,l},{k-1,l}};
        for(const auto& [m,n]:to_try){
            if(m>=0&&n>=0&&m<garden.size()&&n<garden[0].size()&&garden[m][n]=='.'){
                pair<set<pair<int,int>>::iterator,bool> ret_insert;
                if(d%2){
                    ret_insert = visited_even.insert({m,n});
                } else{
                    ret_insert = visited_odd.insert({m,n});
                }
                if(ret_insert.second){
                    queue.push_back({{m,n},d+1});
                }
            }
        }
    }
    int ret_total_odd = visited_odd.size();
    int input_part_2 = 26501365;
    int number_of_garden_radius = (26501365-65)/131; // there are this number of garden right/left/down/up
    long long number_of_garden_area = 1 + number_of_garden_radius * (number_of_garden_radius+1)*2;

    long long ret_2 = number_of_garden_area * ret_total_odd - (ret_total_odd - 3459);

    cout << "Part 1: " << ret_1 << endl;
    cout << "Part 2: " << ret_2 << endl;

    // 1809101443655 too low 

    return 0;
}