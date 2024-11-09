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

void process_line(vector<long long>& input_values, string& line){
    string word;
    istringstream record(line);
    input_values.clear();
    while (record >> word){
        input_values.push_back(stoll(word));
    }
}

struct Pyramid{
    vector<vector<long long>> values;
    long long add_value(const long long&);
    long long next_value();
};

long long Pyramid::add_value(const long long& value){
    if (values.size()==0){
        values.push_back({value});
    } else {
        values[0].push_back(value);
        values.emplace_back();
        for(unsigned i = 1;i<values.size();++i){
            values[i].push_back(values[i-1][values[i-1].size()-2]-values[i-1].back());
        }
    }
    return values.back().back();
}

long long Pyramid::next_value(){
    long long new_value = 0;
    for(auto ite = values.rbegin();ite<values.rend();++ite){
        new_value += (*ite)[0];
    }
    return new_value;
}

long long next_value(const vector<long long>& values, std::ofstream & output){
    Pyramid p;
    bool keep_going=true;
    unsigned i = 0;
    long long value;
    while(keep_going && i < values.size() ){
        value =values[values.size()-1-i++];
        keep_going = p.add_value(value)!=0;
    }
    long long ret = p.next_value();
    output << ret << " " << values.size() << " " << p.values.size() << " " << p.values.back()[0] << endl;
    return ret;
}

long long get_next_value_part_1(const vector<long long>& values){
    if(values.size()>0){
        vector<long long> diff;
        for(unsigned i = 0; i<values.size()-1; ++i){
            diff.push_back(values[i+1]-values[i]);
        }
        return values.back() + get_next_value_part_1(diff);
    } else {
        return 0;
    }
}

long long get_next_value_part_2(const vector<long long>& values){
    if(values.size()>0){
        vector<long long> diff;
        for(unsigned i = 0; i<values.size()-1; ++i){
            diff.push_back(values[i+1]-values[i]);
        }
        return values.front() - get_next_value_part_2(diff);
    } else {
        return 0;
    }
}
int main(){
    std::ifstream file("input");
    std::ofstream ouput("output");
    string line;
    long long res_part_1 = 0;
    long long res_part_2 = 0;
    vector<long long> input_values;
    while(getline(file,line)){
        process_line(input_values,line);
        //res += next_value(input_values,ouput);
        res_part_1 += get_next_value_part_1(input_values);
        res_part_2 += get_next_value_part_2(input_values);
    }
    file.close();
    cout<< "Part 1: The sum of these extrapolated values: " << res_part_1 << endl; 
    cout<< "Part 2: The sum of these extrapolated values: " << res_part_2 << endl; 
    return 0;
}