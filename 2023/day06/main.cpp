#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::sqrt;

vector<int> process_line(string& line){
    string word;
    istringstream record(line);
    vector<int> values;
    record >> word;
    while (record >> word){
        values.push_back(stoi(word));
    }
    return values;
}

struct Race{
    Race() = default;
    Race(const unsigned& t,const unsigned& d):time(t),distance(d){}
    int compute_delta(){return time*time - 4 * distance;}
    int number_of_ways();
    unsigned int time;
    unsigned int distance;
};

int Race::number_of_ways(){
    int delta = this->compute_delta();
    if (delta<0){
        return 0;
    } else if (delta == 0 ){
        return 1;
    } else {
        double x_1 = (time-sqrt(delta))/2;
        double x_2 = (time+sqrt(delta))/2;
        int res = std::floor(x_2)-std::ceil(x_1)+1;
        if( std::floor(x_2) == x_2 ){
            --res;
        }
        if( std::ceil(x_1) == x_1 ){
            --res;
        }
        return res;
    }
}

int main(){
    std::ifstream file("input");
    string line;
    Race race;
    unsigned int res = 1;
    vector<vector<int>> input_values;
    while(getline(file,line)){
        input_values.push_back(process_line(line));
    }
    for (unsigned i = 0; i < input_values[0].size(); ++i){
        Race race(input_values[0][i],input_values[1][i]);
        int temp = race.number_of_ways();
        res *= temp;
        cout<<"Race: " << i << endl;
        cout<<"Number of ways: " << temp << endl;
    }
    file.close();
    cout<< "The product of the number of ways you can beat the record is: " << res << endl; 
    return 0;
}