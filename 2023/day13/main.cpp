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

unsigned find_symmetries(vector<vector<char>> const& pattern){
    int i=0,j=1,k,l;
    bool symetric;
    while(j<pattern.size()){
        if(pattern[i]==pattern[j]){
            k=i,l=j;
            symetric = true;
            while( k>=0 && l < pattern.size() ){
                if(pattern[k]!=pattern[l]){
                    symetric = false;
                    break;
                }
                --k;
                ++l;
            }
            if(symetric){
                return i+1;
            }
        } 
        ++i;
        ++j;
    }
    return 0;
}
int main(){
    std::ifstream file("input");
    std::ofstream output("output");
    string line;
    vector<vector<char>> pattern_lines;
    vector<vector<char>> pattern_columns;
    unsigned sum = 0;
    while(getline(file,line)){
        if(line!=""){
            pattern_lines.push_back(process_line(line));
        } else{
            pattern_columns = build_pattern_columns(pattern_lines);
            sum += find_symmetries(pattern_columns);
            sum += 100*find_symmetries(pattern_lines);
            pattern_lines.clear();
        }
    }
    file.close();
    cout << "The sum of the counts is " << sum << endl;
    return 0;
}