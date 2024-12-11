#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::stringstream;

bool check_if_possible(long long & target,long long  current_number, list<long long> numbers){
    if(current_number>target){
        return false;
    }
    if(!numbers.size()){
        return target==current_number;
    }
    long long front = numbers.front();
    numbers.pop_front();
    return check_if_possible(target,current_number+front,numbers) || check_if_possible(target,(current_number?current_number:1)*front,numbers);
}
int main(){
    std::ifstream file("input");
    string line;
    string sub_line;
    size_t colon_index;
    long long ret_1 = 0;
    long long ret_2 = 0;
    list<long long > numbers;
    long long  target;
    while(getline(file,line)){
        numbers.clear();
        colon_index = line.find(':');
        string sub_line = line.substr(0,colon_index);
        target = stoll(sub_line);
        stringstream ss(line.substr(colon_index + 1));
        while(ss>>sub_line){
            numbers.push_back(stoll(sub_line));
        }
        if(check_if_possible(target,0, numbers)){
            ret_1 += target;
        }
    }
    cout << "Part 1: " << ret_1 << endl; 
    cout<< "Part 2: " << ret_2 << endl;  
    return 0;
}