#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::set;
using std::map;

int part_2(const vector<string>& v){
    map<char,int> seen;
    set<char> can_see;
    //vector<set<char>> first_half(v.size());
    //vector<set<char>> second_half(v.size());
    char key;
    for(int k = 0; k < v.size(); ++k){
        const string& line = v[k];
        can_see.clear();
        for(int i = 0; i < line.size(); ++i){
            const char& c = line[i];
            /*
            if(i < line.size()/2)
                first_half[k].insert(c);
            else {
                second_half[k].insert(c);
            }*/
            if(can_see.insert(c).second){
                ++seen[c];
                if(seen[c] == 3)
                    key = c;
            }
        }
    }
    int total = 0,score = 0;
    if(isupper(key))
        score = key - 'A' + 27;
    else
        score = key - 'a' + 1;
    /*
    for(int i = 0; i < 3; ++i){
       if((first_half[i].find(key) != first_half[i].end() && second_half[i].find(key) == second_half[i].end()) 
            && (first_half[i].find(key) == first_half[i].end() && second_half[i].find(key) != second_half[i].end()))
           total += score;
    }*/
    return score;
}
int main(){
    std::ifstream file("input");
    string line;
    int total_1 = 0,total_2 = 0;
    set<char> seen;
    vector<string> groups;
    while(getline(file,line)){
        groups.push_back(line);    
        if(groups.size() == 3){
           total_2 += part_2(groups);
           groups.clear();
        } 
        seen.clear();
        for(int i = 0; i < line.size(); ++i){
            const char& c = line[i];
            if(i < line.size()/2){
                seen.insert(c);
            } else {
                if(seen.find(c) != seen.end()){
                    seen.erase(c);
                    if(isupper(c))
                        total_1 += c - 'A' + 27; // Uppercase letters: A-Z -> 27-52
                    else 
                        total_1 += c - 'a' + 1; // Lowercase letters: a-z -> 1-26
                }
            }
        }
    }
    file.close();
    cout<< "Part 1: "<< total_1 << endl; 
    cout<< "Part 2: "<< total_2 << endl; 
    return 0;
}