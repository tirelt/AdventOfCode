
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

void print(vector<vector<char>>& garden,set<pair<int,int>>& visited,string name){
    std::ofstream file("output_"+name);
    vector<vector<char>> garden_visited(garden);
    for(const auto& [i,j]:visited){
        garden_visited[i][j] = 'O';
    }
    for(const auto& line:garden_visited){
        for(const auto& c:line)
            file << c;
        file << endl;
    }
}

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
    //i_0 = 130,j_0=130; change start

    list<pair<pair<int,int>,int>> queue{{{i_0,j_0},0}};
    set<pair<int,int>> visited_even{{i_0,j_0}};
    set<pair<int,int>> visited_odd;

    int depth_part1 = 64;
    int ret_1 = 0;
    bool locked = false;

    int depth_65 = 65;
    int ret_65_steps = 0;
    bool locked_65 = false;

    while(queue.size()){ // we go through all the garden
        if(!locked && queue.front().second==depth_part1){ //we same when current depth reach the target
            ret_1 = depth_part1%2?visited_odd.size():visited_even.size();
            locked = true;
        }
        if(!locked_65 && queue.front().second==depth_65){
            ret_65_steps = depth_65%2?visited_odd.size():visited_even.size();
            locked_65 = true;
            //print(garden,visited_odd,"65_corner");
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
    /*
    Geometrical master class (c.f. saved outputs).
    Relying on the fact that the inputs present interesting symmetries 
    */
    int input_part_2 = 26501365; //number of steps 
    int number_of_garden_radius = (26501365-65)/131; // there are this number of garden right/left/down/up from the central garden.
    long long N = number_of_garden_radius / 2; //even number so this is fine. Can refine to generalize the logic
    long long number_of_garden_odd = 1+4*N*(N+1); //because the number of steps is odd, the first garden is an state in odd state. All the every other garden are the same state
    long long number_of_garden_even= 4*N*N; //formalas from simple recurrence.

    long long ret_2 = number_of_garden_odd*visited_odd.size()+ number_of_garden_even*visited_even.size() + number_of_garden_radius *(visited_even.size() - ret_1) - (number_of_garden_radius+1) * (visited_odd.size()-ret_65_steps);
    //we take the number of gardens in odd state,the number of gardnes in even state. The corners of the mostly covered gardens 4*(radius + 1) are odd an needs to be take out. It is a multiple of 4 so we can divide, this number is full odd garden - 65 steps.
    //the corners from gadens not mostly covered 4*radius. Again we group them in full garden and substract the 64 steps from the full even garden.

    cout << "Part 1: " << ret_1 << endl;
    cout << "Part 2: " << ret_2 << endl;

    return 0;
}