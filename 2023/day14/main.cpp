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
void roll_south(vector<char>& col){
    unsigned possible_pos = col.size()-1;
    for(int i=col.size()-1;i>=0;--i){
        if(col[i]=='O'){
            if(i != possible_pos){
                col[i]='.';
                col[possible_pos]='O';
            }
            --possible_pos; 
        }
        if(col[i]=='#'){
            possible_pos = i-1;
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

unsigned cycle(vector<vector<char>> &pattern_lines){
    vector<vector<char>> pattern = build_pattern_columns(pattern_lines); // Col
    for(vector<char>& c : pattern){
            roll_north(c);
    }
    pattern = build_pattern_columns(pattern); // Line
    for(vector<char>& c : pattern){
            roll_north(c);
    }
    pattern = build_pattern_columns(pattern); // Col
    for(vector<char>& c : pattern){
            roll_south(c);
    }
    pattern = build_pattern_columns(pattern); // Line
    for(vector<char>& c : pattern){
            roll_south(c);
    }
    pattern_lines = pattern;

    unsigned load = 0;
    unsigned load_temp = 0;
    vector<char> l;
    for(unsigned i = 0;i<pattern.size();++i){
        l = pattern[i];
        load_temp = 0;
        for(char& c : l){
            if(c=='O')
                ++load_temp;
        }
        load += load_temp*(pattern.size()-i);
    }
    return load;
}

int main(){
    std::ifstream file("input");
    std::ofstream output("output");
    string line;
    vector<vector<char>> pattern_lines;
    while(getline(file,line)){
        pattern_lines.push_back(process_line(line));
    }
    file.close();
    unsigned load;
    vector<vector<char>> pattern_9;
    vector<vector<char>> pattern_16;
    // example 
    /*
    for(unsigned i = 1;i<30;++i){
        load = cycle(pattern_lines);
        if(i==9){
            pattern_9 = pattern_lines;
        }
        if(i==16){
            pattern_16 = pattern_lines;
        }
        cout << "Cycle " << i << " " << load << endl;
    }
    cout << "pattern_9 == pattern_16 :" << (pattern_9 ==  pattern_16) << endl;
    vector<unsigned> cycle_values{68,69,69,65,64,65,63}; // we notice loops
    cout << "The total load on the north support beams is " << cycle_values[(1000000000-9)%7] << endl;
    */
    for(unsigned i = 1;i<1000;++i){
        load = cycle(pattern_lines);
        if(i==971){
            pattern_9 = pattern_lines;
        }
        if(i==997){
            pattern_16 = pattern_lines;
        }
        //cout << "Cycle " << i << " " << load << endl;
        output << load << endl;
    }
    cout << "pattern_971 == pattern_997 :" << (pattern_9 ==  pattern_16) << endl;
    vector<unsigned> cycle_values{104988,104983,104987,105008,105060,105112,105154,105197,105255,105312,105351,105354,105358,105362,105322,105299,105283,105265,105225,105191,105162,105133,105097,105078,105037,105000}; // we notice loops
    cout << "The total load on the north support beams is " << cycle_values[(1000000000-971)%(997-971)] << endl;
    return 0;
}