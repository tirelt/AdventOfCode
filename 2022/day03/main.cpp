#include <fstream>
#include <iostream>
#include <string>
#include <set>

using std::string;
using std::cout;
using std::endl;
using std::set;

int main(){
    std::ifstream file("input");
    string line;
    int total_1 = 0,total_2 = 0;
    set<char> seen;
    while(getline(file,line)){
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