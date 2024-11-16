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
#include <tuple>

using std::cout;
using std::endl;
using std::vector;
using std::deque;
using std::string;
using std::istringstream;
using std::accumulate;
using std::copy;
using std::map;
using std::tuple;

void process_line(string& line, vector<char>& status, vector<unsigned>& damaged_group,deque<vector<char>>& to_insert,map<unsigned,char>& status_to_check){
    bool has_seen_separator = false;
    status.clear();
    damaged_group.clear();
    to_insert.clear();
    status_to_check.clear();
    unsigned sized = 0;
    vector<char> temp;
    for(unsigned i=0;i<5;++i){
        auto ite = line.begin();
        while( *ite != ' ' ){
            status.push_back(*ite);
            if(*ite!='?') status_to_check.insert({ite-line.begin()+sized,*ite});
            ++ite;
        }
        status.push_back('?');
        sized = status.size();
        ++ite;

        string sub_line(ite,line.end()),word;
        istringstream record(sub_line);
        while(getline(record,word,',')){
            damaged_group.push_back(stoi(word));
            temp = vector<char>(damaged_group.back(),'#');
            temp.push_back('.');
            to_insert.push_back(temp);
        }
    }
    status.pop_back();
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

using key_type = tuple<deque<vector<char>>::iterator,deque<vector<char>>::iterator,vector<char>::iterator,vector<char>::iterator> ;

bool check_can_insert(vector<char>& to_insert,vector<char>::iterator& status_start){
    bool ret = true;
    for(unsigned i=0;i<to_insert.size();++i){
        ret = ret && (to_insert[i] == *status_start || *status_start == '?');
        ++status_start;
    }
    return ret;
}

long long unsigned count_possibilities_smart(deque<vector<char>>::iterator to_insert_start,deque<vector<char>>::iterator to_insert_end,vector<char>::iterator status_start, vector<char>::iterator status_end,map<unsigned,long long unsigned>& memory){
    if(to_insert_start==to_insert_end){
        while(status_start!=status_end){
            if(*status_start++=='#'){
                return 0;
            }
        }
        return 1;
    }
    unsigned key = (to_insert_end-to_insert_start) + (status_end-status_start)*1000;
    if(memory.find(key)!=memory.end()){
        return memory[key];
    } 
    long long unsigned sum=0;
    vector<char>::iterator ite;
    vector<char> to_insert = *to_insert_start++;
    while(status_start!=status_end && (status_end-status_start)>=to_insert.size()){
        ite = status_start;
        if(check_can_insert(to_insert,ite)){
            key = (to_insert_end-to_insert_start) + (status_end-ite)*1000;
            memory[key] = count_possibilities_smart(to_insert_start,to_insert_end,ite,status_end,memory);
            sum += memory[key];
        }
        if(*status_start=='#'){
            break;
        }
        ++status_start;
    }
    return sum;
}

int main(){
    std::ifstream file("input");
    std::ofstream output("output");
    string line;
    vector<char> status;
    vector<unsigned> damaged_group;
    deque<vector<char>> to_insert;
    map<unsigned,char> status_to_check;
    unsigned call_counter=0,number_of_extra_status,ret, sum = 0,counter = 0;
    map<unsigned,long long unsigned> memory;
    while(getline(file,line)){
        call_counter = 0;
        process_line(line,status,damaged_group,to_insert,status_to_check);
        number_of_extra_status = (status.size()-(accumulate(damaged_group.begin(),damaged_group.end(),0)+damaged_group.size()-1));
        //ret = count_possibilities(to_insert,{},number_of_extra_status,status_to_check, call_counter);
        memory.clear();
        ret = count_possibilities_smart(to_insert.begin(),to_insert.end(),status.begin(), status.end(),memory);
        //cout << ++counter << endl;
        output << number_of_possibilities(status, damaged_group)  << " " << memory.size()<< " "<< ret << endl;
        sum += ret;
    }
    file.close();
    cout << "The sum of the counts is " << sum << endl;
    return 0;
}