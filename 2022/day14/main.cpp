#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::stoi;

int main(){
    std::ifstream file("input");
    std::regex number_regex("\\d+");
    string line;
    vector<vector<pair<int, int>>> rocks;
    vector<int> all_x, all_y;
    while(getline(file,line)){
        auto begin = std::sregex_iterator(line.begin(), line.end(), number_regex);
        auto end = std::sregex_iterator();
        vector<pair<int, int>> rock;
        for (std::sregex_iterator i = begin; i != end; ++++i) {
            auto next = i;++next;
            rock.emplace_back(stoi(i->str()), stoi(next->str()));
            all_x.push_back(stoi(i->str()));
            all_y.push_back(stoi(next->str()));
        }
        rocks.push_back(rock);
    }
    int min_x = *std::min_element(all_x.begin(), all_x.end());
    int max_x = *std::max_element(all_x.begin(), all_x.end());
    int min_y = 0; //*std::min_element(all_y.begin(), all_y.end());
    int max_y = *std::max_element(all_y.begin(), all_y.end());
    
    vector<vector<char>> grid(max_y - min_y + 1, vector<char>(max_x - min_x + 1, '.'));
    for (const auto& rock : rocks) {
        for (int i = 0; i < rock.size() - 1; ++i) {
            grid[rock[i].second - min_y][rock[i].first - min_x] = '#';
            if(i+1 < rock.size()) {
                int x1 = rock[i].first - min_x;
                int y1 = rock[i].second - min_y;
                int x2 = rock[i + 1].first - min_x;
                int y2 = rock[i + 1].second - min_y;
                if (x1 == x2) { // vertical line
                    for (int j = std::min(y1, y2); j <= std::max(y1, y2); ++j) {
                        grid[j][x1] = '#';
                    }
                } else if (y1 == y2) { // horizontal line
                    for (int j = std::min(x1, x2); j <= std::max(x1, x2); ++j) {
                        grid[y1][j] = '#';
                    }
                }
            }
        }
    }
    int ret_1 = 0;
    while (true) {
        int x = 500 - min_x;
        int y = 0;
        bool escaped = false;
        while (y < grid.size() ) {
            if (y+1 == grid.size()){
                escaped = true; 
                break;
            } else if (grid[y + 1][x] == '.') {
                y++;
            } else if( x == 0 ){
                escaped = true;
                break;
            } else if (grid[y + 1][x - 1] == '.') {
                y++;
                x--;
            } else if (x == grid[0].size() - 1) {
                escaped = true;
                break;
            } else if (grid[y + 1][x + 1] == '.') {
                y++;
                x++;
            } else {
                break;
            }
        }
        if (escaped) {
            break; 
        }
        grid[y][x] = 'o';
        ret_1++;
    }
    cout<< "Part 1: " << ret_1 << endl; 
    
    // Part 2
    max_y += 2;
    min_x = std::min(min_x, 500 - max_y);
    max_x = std::max(max_x, 500 + max_y);
    vector<vector<char>> grid_2(max_y-min_y+1, vector<char>(max_x - min_x + 1, '.'));
    grid = grid_2; 
    for (const auto& rock : rocks) {
        for (int i = 0; i < rock.size() - 1; ++i) {
            grid_2[rock[i].second - min_y][rock[i].first - min_x] = '#';
            if(i+1 < rock.size()) {
                int x1 = rock[i].first - min_x;
                int y1 = rock[i].second - min_y;
                int x2 = rock[i + 1].first - min_x;
                int y2 = rock[i + 1].second - min_y;
                if (x1 == x2) { // vertical line
                    for (int j = std::min(y1, y2); j <= std::max(y1, y2); ++j) {
                        grid[j][x1] = '#';
                    }
                } else if (y1 == y2) { // horizontal line
                    for (int j = std::min(x1, x2); j <= std::max(x1, x2); ++j) {
                        grid[y1][j] = '#';
                    }
                }
            }
        }
    }
    for (int i = 0; i < grid[0].size(); ++i) {
        grid[max_y - min_y][i] = '#'; // Add the floor
    }
    int ret_2 = 0;
    while (true) {
        int x = 500 - min_x;
        int y = 0;
        bool escaped = false;
        while (y < grid.size() ) {
            if (y+1 == grid.size()){
                escaped = true; 
                break;
            } else if (grid[y + 1][x] == '.') {
                y++;
            } else if( x == 0 ){
                escaped = true;
                break;
            } else if (grid[y + 1][x - 1] == '.') {
                y++;
                x--;
            } else if (x == grid[0].size() - 1) {
                escaped = true;
                break;
            } else if (grid[y + 1][x + 1] == '.') {
                y++;
                x++;
            } else {
                break;
            }
        }
        if (escaped || (y == 0 && x == 500 - min_x)) {
            break; 
        }
        grid[y][x] = 'o';
        ret_2++;
    }
    cout<< "Part 2: " << ret_2+1 << endl; 

    return 0;
}