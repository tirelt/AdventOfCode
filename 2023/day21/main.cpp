
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
    while(queue.size() && queue.front().second<max_depth){
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
    int ret_1 = max_depth%2?visited_odd.size():visited_even.size();
    cout << "Part 1: " << ret_1 << endl;
    cout << "Part 2: " << 0 << endl;
    return 0;
}