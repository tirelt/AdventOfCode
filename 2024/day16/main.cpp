#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::list;

int main(){
    std::ifstream file("test_input");
    string line;
    vector<vector<char>> maze;
    int i=0,j=0,x,y;
    while(getline(file,line)){
        vector<char> temp;
        j = 0;
        for(const char& c:line){
            temp.emplace_back(c);
            if(c=='S'){
                x=i;
                y=j;
            }
            ++j;
        }
        maze.emplace_back(temp);
        ++i;
    }
    cout<< "Part 1: " << 0 << endl;       return 0;
}