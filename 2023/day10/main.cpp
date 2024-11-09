#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::pair;
using std::max;
using std::min;
using std::find;
vector<char> process_line(string& line,unsigned& i_S, unsigned& j_S,unsigned& i, unsigned& j){
    vector<char> line_vector;
    bool found_S=false;
    j=0;
    for(const char& c : line){
        line_vector.push_back(c);
        if(c == 'S'){
            i_S = i,j_S = j;
        }
        ++j;
    }
    ++i;
    return line_vector;
}

int find_next(vector<vector<char>>& pipes,vector<char>& go_north,vector<char>& go_west,vector<char>& go_east,vector<char>& go_south,unsigned&i, unsigned& j, char& come_from){
    if(come_from != 'N' && i>0 && find(go_south.begin(),go_south.end(),pipes[i][j])!=go_south.end()){
        auto ret = find(go_north.begin(),go_north.end(),pipes[i-1][j]);
        if(ret!=go_north.end()){
            come_from = 'S';
            --i;
            return 0;
        }
    }
    if(come_from != 'W' && j>0 && find(go_east.begin(),go_east.end(),pipes[i][j])!=go_east.end()){
        auto ret = find(go_west.begin(),go_west.end(),pipes[i][j-1]);
        if(ret!=go_west.end()){
            come_from = 'E';
            --j;
            return 0;
        } 
    }
    if(come_from != 'S' && i<pipes.size()-1 && find(go_north.begin(),go_north.end(),pipes[i][j])!=go_north.end()){
        auto ret = find(go_south.begin(),go_south.end(),pipes[i+1][j]);
        if(ret!=go_south.end()){
            come_from = 'N';
            ++i;
            return 0;
        }
    } 
    if(come_from != 'E' && j<pipes[0].size()-1 && find(go_west.begin(),go_west.end(),pipes[i][j])!=go_west.end()){
        auto ret = find(go_east.begin(),go_east.end(),pipes[i][j+1]);
        if(ret!=go_east.end()){
            come_from = 'W';
            ++j;
            return 0;
        }
    }
    throw std::runtime_error("STUCK!!");
}

int main(){
    std::ifstream file("input");
    string line;
    vector<vector<char>> pipes;
    unsigned i=0,j=0,i_S=0,j_S=0;
    while(getline(file,line)){
        pipes.push_back(process_line(line,i_S,j_S,i,j));
    }
    file.close();

    vector<char> go_north{'S','|','7','F'};
    vector<char> go_west{'S','F','-','L'};
    vector<char> go_east{'S','7','-','J'};
    vector<char> go_south{'S','|','L','J'};
    char come_from = '.';

    unsigned counter = 0;
    i = i_S, j = j_S;
    while( (pipes[i][j]!='S' || counter == 0) && counter < 20000 ){
        find_next(pipes,go_north,go_west,go_east,go_south,i,j,come_from);    
        ++counter;
    }
    cout<< "The fr=arthest point is " <<  counter/2 << " steps from the starting position"<< endl; 
    return 0;
}