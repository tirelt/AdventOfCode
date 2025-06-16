#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <deque>

using std::string;
using std::cout;
using std::endl;
using std::set;
using std::deque;

int aux(const string& line,int n){
    int res_1 = 0;
    deque<char> last_four;
    for(const char& c : line){
        ++res_1;
        last_four.push_back(c);
        if(last_four.size()>=n){
            set<char> seen(last_four.begin(), last_four.end());
            if(seen.size() == n)
                break;
            last_four.pop_front();
        }        
    }
    return res_1;
}
int main(){
    std::ifstream file("input");
    string line;
    int res_1 = 0, res_2 = 0;
    while(getline(file,line)){
        res_1 += aux(line,4);
        res_2 += aux(line,14);
    }
    file.close();

    cout<< "Part 1: " << res_1 << endl; 
    cout<< "Part 2: " << res_2 << endl; 
    return 0;
}