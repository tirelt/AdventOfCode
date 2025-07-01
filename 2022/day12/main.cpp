#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <queue>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::map;
using std::priority_queue;

struct Position {
    pair<int, int> coords;
    int num_steps;
    bool operator<(const Position& other) const {
        return num_steps > other.num_steps;
    }
};

int main(){
    std::ifstream file("input");
    string line;
    vector<vector<char>> height_map;
    priority_queue<Position> queue;
    int i = 0, j = 0;
    pair<int, int> end_pos;
    map<pair<int,int>,int> seen;

    while(getline(file,line)){
        vector<char> row;
        j = 0;
        for(char c : line){
            if(c == 'S'){
                queue.push({{i, j}, 0});
                seen[{i, j}] = 0;
                row.push_back('a'); 
            } else if(c == 'E') {
                end_pos = {i, j};
                row.push_back('z'); 
            } else
                row.push_back(c);
            ++j;
        }
        ++i;
        height_map.push_back(row);
    }
    file.close();
    
    while(!queue.empty() && queue.top().coords != end_pos){
        Position current = queue.top();
        queue.pop();

        int x = current.coords.first;
        int y = current.coords.second;

        for(auto [dx, dy] : vector<pair<int, int>>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}){
            int new_x = x + dx;
            int new_y = y + dy;

            if(new_x < 0 || new_y < 0 || new_x >= height_map.size() || new_y >= height_map[0].size())
                continue;

            char next_height = height_map[new_x][new_y];
            if(next_height - height_map[x][y] <= 1){
                pair<int, int> new_coords = {new_x, new_y};
                if(seen.find(new_coords) == seen.end() || seen[new_coords] > current.num_steps + 1){
                    seen[new_coords] = current.num_steps + 1;
                    queue.push({new_coords, current.num_steps + 1});
                }
            }
        }
    }
    cout<< "Part 1: " << queue.top().num_steps << endl; 

    // Reset for part 2
    queue = priority_queue<Position>();
    queue.push({{end_pos.first, end_pos.second}, 0});
    seen.clear();
    seen[{end_pos.first, end_pos.second}] = 0;
    while(!queue.empty() && height_map[queue.top().coords.first][queue.top().coords.second] != 'a'){
        Position current = queue.top();
        queue.pop();

        int x = current.coords.first;
        int y = current.coords.second;

        for(auto [dx, dy] : vector<pair<int, int>>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}){
            int new_x = x + dx;
            int new_y = y + dy;

            if(new_x < 0 || new_y < 0 || new_x >= height_map.size() || new_y >= height_map[0].size())
                continue;

            char next_height = height_map[new_x][new_y];
            if(next_height - height_map[x][y] >= -1){
                pair<int, int> new_coords = {new_x, new_y};
                if(seen.find(new_coords) == seen.end() || seen[new_coords] > current.num_steps + 1){
                    seen[new_coords] = current.num_steps + 1;
                    queue.push({new_coords, current.num_steps + 1});
                }
            }
        }
    }
    cout<< "Part 2: " << queue.top().num_steps << endl; 

    return 0;
}