#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::array;
using std::map;

int main(){
    std::ifstream file("input");
    string line;
    vector<int> temp{-1,-1,-1,-1,-1};
    vector<vector<int>> keys;
    vector<vector<int>> locks;
    int count_first_line=0;
    int check_first=true;
    while(getline(file,line)){
        if(!line.size()){
            if(count_first_line==temp.size()) locks.push_back(temp);
            else keys.push_back(temp);
            check_first = true,count_first_line=0;
            for(int& e:temp) e = -1;
        } else{
            for(int i = 0; i<temp.size();++i){
                temp[i] += line[i]=='#';
            }
            if(check_first){
                check_first = false;
                for(const char& c:line) if( c == '#') ++count_first_line;
            }      
        }
    }
    if(count_first_line==temp.size()) locks.push_back(temp);
    else keys.push_back(temp);
    check_first = true,count_first_line=0;

    int ret_1 = 0;
    bool add = true;
    for(const auto& key:keys){
        for(const auto& lock:locks){
            add = true;
            for(int i=0;i<temp.size();++i){
                add &= key[i]+lock[i]<=temp.size();
            }
            if(add) ++ret_1;
        }
    }
    cout << "Part 1: " << ret_1 << endl;
    return 0;
}
