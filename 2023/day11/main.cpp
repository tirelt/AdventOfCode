#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::fabs;

struct Galaxy{
    Galaxy() = default;
    Galaxy(unsigned x,unsigned y):i(x),j(y),i_init(x),j_init(y){};
    unsigned i;
    unsigned j;
    unsigned i_init;
    unsigned j_init;
};

vector<char> process_line(string& line){
    vector<char> line_vector;
    for(const char& c : line){
        line_vector.push_back(c);
    }
    return line_vector;
}
int main(){
    std::ifstream file("input");
    string line;
    vector<vector<char>> galaxies_map;
    vector<Galaxy> galaxies_list;
    while(getline(file,line)){
        galaxies_map.push_back(process_line(line));
    }
    file.close();
    bool empty_line;
    vector<unsigned> empty_lines;
    for(unsigned i = 0;i<galaxies_map.size();++i){
        empty_line = true;
        for(unsigned j = 0;j<galaxies_map[0].size();++j){
            empty_line = empty_line && galaxies_map[i][j] != '#'; 
            if(galaxies_map[i][j] == '#')
                galaxies_list.emplace_back(i,j);
        }
        if(empty_line){
            empty_lines.push_back(i);
        }
    }
    bool empty_column;
    vector<unsigned> empty_columns;
    for(unsigned j = 0;j<galaxies_map[0].size();++j){
        empty_column = true;
        for(unsigned i = 0;i<galaxies_map.size();++i){
            empty_column = empty_column && galaxies_map[i][j] != '#'; 
        }
        if(empty_column){
            empty_columns.push_back(j);
        }
    }
    for(unsigned i: empty_lines){
        for(Galaxy& g :galaxies_list){
            if(g.i_init>i)
                g.i+=1;
        }
    }
    for(unsigned j: empty_columns){
        for(Galaxy& g :galaxies_list){
            if(g.j_init>j)
                g.j+=1;
        }
    }
    Galaxy g,h;
    vector<vector<unsigned>> distances(galaxies_list.size(),vector<unsigned>(galaxies_list.size()));
    unsigned dist_total = 0;
    for(unsigned k=0;k<galaxies_list.size();++k){
        g = galaxies_list[k];
        for(unsigned l=k+1;l<galaxies_list.size();++l){
            h = galaxies_list[l];
            dist_total += abs(int(g.i)-int(h.i))+abs(int(g.j)-int(h.j));
            distances[k][l] = abs(int(g.i)-int(h.i))+abs(int(g.j)-int(h.j));
        }
    }
    cout << "The sum of the lengths is: " << dist_total << endl;
    return 0;
}