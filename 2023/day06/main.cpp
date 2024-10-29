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

vector<long long> process_line(string& line){
    string word;
    istringstream record(line);
    vector<long long> values;
    record >> word;
    while (record >> word){
        values.push_back(stoll(word));
    }
    return values;
}

struct Race{
    Race() = default;
    Race(const long long& t,const long long& d):time(t),distance(d){}
    long long compute_delta(){return time*time - 4 * distance;}
    long long number_of_ways();
    unsigned long long time;
    unsigned long long distance;
};

long long Race::number_of_ways(){
    long long delta = this->compute_delta();
    if (delta<0){
        return 0;
    } else if (delta == 0 ){
        return 1;
    } else {
        long double x_1 = (time-sqrt(delta))/2;
        long double x_2 = (time+sqrt(delta))/2;
        long long res = std::floor(x_2)-std::ceil(x_1)+1;
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
    std::ifstream file("input_part2");
    string line;
    Race race;
    unsigned long long res = 1;
    vector<vector<long long>> input_values;
    while(getline(file,line)){
        input_values.push_back(process_line(line));
    }
    for (unsigned i = 0; i < input_values[0].size(); ++i){
        Race race(input_values[0][i],input_values[1][i]);
        long long temp = race.number_of_ways();
        res *= temp;
        cout<<"Race: " << i << endl;
        cout<<"Number of ways: " << temp << endl;
    }
    file.close();
    cout<< "The product of the number of ways you can beat the record is: " << res << endl; 
    return 0;
}