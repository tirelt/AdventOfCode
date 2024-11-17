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

unsigned hash(string const& s){
    unsigned code = 0;
    for(char const& c : s){
        code += int(c);
        code *= 17;
        code = code % 256;
    }
    return code;
}

int main(){
    std::ifstream file("input");
    string line;
    string word;
    unsigned code;
    unsigned ret = 0;
    while(getline(file,line,',')){
        code = hash(line);
        //cout << code << endl;
        ret += code;
    }
    file.close();
    cout << "The sum of the results is " << ret << endl;
    return 0;
}