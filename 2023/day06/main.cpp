#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;

vector<unsigned int> process_line(string& line){
    string word;
    istringstream record(line);
    vector<unsigned int> values;
    record >> word;
    while (record >> word){
        cout<< word << endl; 
    }
    return values;
}

int main(){
    std::ifstream file("input");
    string line;
    unsigned int res = 1;
    vector<vector<unsigned int>> input_values;
    while(getline(file,line)){
        input_values.push_back(process_line(line));
    }
    file.close();
    cout<< "The product of the number of ways you can beat the record is: "<< res << endl; 
    return 0;
}