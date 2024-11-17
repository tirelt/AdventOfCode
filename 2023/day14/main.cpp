#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <string>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::deque;
using std::string;
using std::istringstream;
using std::map;

vector<char> process_line(string& line){
    vector<char> ret;
    for(char const& c : line){
        ret.push_back(c);
    }
    return ret;
}

vector<vector<char>> build_pattern_columns(vector<vector<char>>& pattern_lines){
    vector<vector<char>> pattern_columns(pattern_lines[0].size(),vector<char>(pattern_lines.size()));
    for(unsigned i=0;i<pattern_lines.size();++i){
         for(unsigned j=0;j<pattern_lines[0].size();++j){
            pattern_columns[j][i]=pattern_lines[i][j];
         }
    }
    return pattern_columns;
}

void roll_north(vector<char>& col){
    unsigned possible_pos = 0;
    for(unsigned i=0;i<col.size();++i){
        if(col[i]=='O'){
            if(i != possible_pos){
                col[i]='.';
                col[possible_pos]='O';
            }
            ++possible_pos; 
        }
        if(col[i]=='#'){
            possible_pos = i+1;
        }
    }
}

unsigned calculate_load(vector<char> const& col){
    unsigned sum = 0;
    for(unsigned i = 0;i<col.size();++i){
        if(col[i]=='O')
            sum += col.size()-i;
    }
    return sum;
}
int main(){
    std::ifstream file("input");
    std::ofstream output("output");
    string line;
    vector<vector<char>> pattern_lines;
    vector<vector<char>> pattern_columns;
    while(getline(file,line)){
        pattern_lines.push_back(process_line(line));
    }
    file.close();
    pattern_columns = build_pattern_columns(pattern_lines);
    unsigned load = 0;
    for(vector<char>& col : pattern_columns){
        roll_north(col);
        load += calculate_load(col);
    }
    cout << "The total load on the north support beams is " << load << endl;
    return 0;
}