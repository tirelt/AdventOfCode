#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::list;
using std::stringstream;

bool part_1(long long & target,long long  current_number, list<long long>& numbers,list<long long >::iterator ite){
    if(current_number>target){
        return false;
    }
    if(ite==numbers.end()){
        return target==current_number;
    }
    long long front = *ite;
    list<long long >::iterator next_ite = std::next(ite);
    return part_1(target,current_number+front,numbers, next_ite)||(current_number?part_1(target,current_number*front,numbers,next_ite):false);
}

long long concat_numbers(long long a, long long b){ 
    long long multiplier = 1; 
    while (b >= multiplier){
        multiplier *= 10;
    } 
    return a * multiplier + b; 
}

bool part_2(long long const & target,long long  current_number, list<long long> const& numbers, list<long long >::iterator ite){
    if(current_number>target){
        return false;
    }
    if(ite==numbers.end()){
        return target==current_number;
    }
    long long front = *ite;
    list<long long >::iterator next_ite = std::next(ite);
    return part_2(target,concat_numbers(current_number,front),numbers, next_ite)||part_2(target,current_number+front,numbers, next_ite)||(current_number?part_2(target,current_number*front,numbers,next_ite):false);
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
        if(part_1(target,0, numbers,numbers.begin())){
            ret_1 += target;
        }
        if(part_2(target,0, numbers,numbers.begin())){
            ret_2 += target;
        }
    }
    cout << "Part 1: " << ret_1 << endl; 
    cout<< "Part 2: " << ret_2 << endl;  
    return 0;
}