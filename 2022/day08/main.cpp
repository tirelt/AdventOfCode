#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;

struct Forest {
    Forest(int h) : height(h), visible(false) {}
    int height;
    bool visible;
};

int main(){
    std::ifstream file("input");
    string line;
    vector<vector<Forest>> forest;
    int res_1 = 0, res_2 = 0;
    while(getline(file,line)){
        vector<Forest> row;
        for(const char& c : line){
            row.emplace_back(c - '0'); 
        }
        forest.push_back(row); 
    }
    for(int i = 0; i < forest.size(); ++i){
        int current_height = -1;
        for(int j = 0; j < forest[i].size(); ++j){
            if(forest[i][j].height > current_height){
                forest[i][j].visible = true;
                current_height = forest[i][j].height;
            }
        }
        current_height = -1;
        for(int j = forest[i].size()-1; j > 0; --j){
            if(forest[i][j].height > current_height){
                forest[i][j].visible = true;
                current_height = forest[i][j].height;
            }
        }
    }
    for(int j = 0; j < forest[0].size(); ++j){
        int current_height = -1;
        for(int i = 0; i < forest.size(); ++i){
            if(forest[i][j].height > current_height){
                forest[i][j].visible = true;
                current_height = forest[i][j].height;
            }
        }
        current_height = -1;
        for(int i = forest.size()-1; i > 0; --i){
            if(forest[i][j].height > current_height){
                forest[i][j].visible = true;
                current_height = forest[i][j].height;
            }
        }
    }
    for(const auto& row : forest){
        for(const auto& t : row){
            if(t.visible)
                ++res_1;
        }
    }
    cout<< "Part 1: " << res_1 << endl; 
    cout<< "Part 2: " << res_2 << endl; 
    return 0;
}