#include <fstream>
#include <iostream>
#include <deque>
#include <string>
#include <set>
#include <sstream>

using std::cout;
using std::endl;
using std::deque;
using std::string;
using std::set;
using std::istringstream;

deque<set<int>>::iterator add(deque<set<int>>& mapping,int& current_x, deque<set<int>>::iterator ite,string direction,int dist){
    
}
int main(){
    std::ifstream file("test_input");
    string line,direction,dist,colour;
    deque<set<int>> mapping{set<int>()};
    mapping[0].insert(0);
    int current_x;
    while(getline(file,line)){
        istringstream entries(line,' ');
        entries >> direction;
        entries >> dist;
        entries >> colour;
    }
    cout << "Part 1  " << 0 << endl;
    return 0;
}