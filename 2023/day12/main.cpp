#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::deque;
using std::string;
using std::istringstream;
using std::accumulate;
using std::copy;
using std::map;

void process_line(string& line, vector<char>& status, vector<unsigned>& damaged_group,deque<vector<char>>& to_insert,map<unsigned,char>& status_to_check){
    bool has_seen_separator = false;
    status.clear();
    damaged_group.clear();
    to_insert.clear();
    status_to_check.clear();
    auto ite = line.begin();
    while( *ite != ' ' ){
        status.push_back(*ite);
        if(*ite!='?') status_to_check.insert({ite-line.begin(),*ite});
        ++ite;
    }
    ++ite;
    string sub_line(ite,line.end()),word;
    istringstream record(sub_line);
    vector<char> temp;
    while(getline(record,word,',')){
        damaged_group.push_back(stoi(word));
        temp = vector<char>(damaged_group.back(),'#');
        temp.push_back('.');
        to_insert.push_back(temp);
    }
    to_insert.back().pop_back();
}

unsigned long long fact(unsigned n){
    if( n ==0 || n == 1 ){
        return 1;
    } else {
        return n *fact(n-1);
    }
}

unsigned long long combin(unsigned n, unsigned k){
    return fact(n)/(fact(k)*fact(n-k));
}
unsigned number_of_possibilities(vector<char>& status, vector<unsigned>& damaged_group){
    unsigned n = (status.size()-(accumulate(damaged_group.begin(),damaged_group.end(),0)+damaged_group.size()-1)) + damaged_group.size();
    unsigned k = damaged_group.size();
    return combin(n, k);
}

bool check_current_status(vector<char>& current_status,map<unsigned,char>& status_to_check){
    auto ite = status_to_check.begin();
    while(ite != status_to_check.end() && ite->first < current_status.size() && current_status[ite->first]==ite->second){
        ++ite;
    }
    return ite == status_to_check.end() || ite->first >= current_status.size();
}
unsigned count_possibilities(deque<vector<char>> to_insert,vector<char> current_status,unsigned number_of_extra_status, map<unsigned,char>& status_to_check, unsigned& call_counter){
    call_counter += 1;
    if(to_insert.size()>0){
        unsigned sum = 0;
        for(unsigned i = 0;i<=number_of_extra_status;++i){
            deque<vector<char>> to_insert_bis = to_insert;
            vector<char> current_status_bis = current_status;
            for(unsigned j=0;j<i;++j) current_status_bis.push_back('.');
            copy(to_insert_bis.front().begin(),to_insert_bis.front().end(),back_inserter(current_status_bis));
            to_insert_bis.pop_front();
            if(check_current_status(current_status_bis,status_to_check)){
                sum += count_possibilities(to_insert_bis,current_status_bis,number_of_extra_status-i, status_to_check,call_counter);
            }
        }    
        return sum;
    } else {
        if(number_of_extra_status){
            for(unsigned j=0;j<number_of_extra_status;++j) current_status.push_back('.');
        }
        return check_current_status(current_status,status_to_check);
    }
}
int main(){
    std::ifstream file("input");
    std::ofstream output("output");
    string line;
    vector<char> status;
    vector<unsigned> damaged_group;
    deque<vector<char>> to_insert;
    map<unsigned,char> status_to_check;
    unsigned call_counter=0,number_of_extra_status,ret, sum = 0;
    while(getline(file,line)){
        call_counter = 0;
        process_line(line,status,damaged_group,to_insert,status_to_check);
        number_of_extra_status = (status.size()-(accumulate(damaged_group.begin(),damaged_group.end(),0)+damaged_group.size()-1));
        ret = count_possibilities(to_insert,{},number_of_extra_status,status_to_check, call_counter);
        output << number_of_possibilities(status, damaged_group)  << " " << ret << " " << call_counter << endl;
        sum += ret;
    }
    file.close();
    cout << "The sum of the counts is " << sum << endl;
    return 0;
}