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

void process_line(string& line, vector<char>& status,deque<vector<char>>& to_insert){
    bool has_seen_separator = false;
    status.clear();
    to_insert.clear();
    unsigned sized = 0;
    vector<char> temp;
    for(unsigned i=0;i<5;++i){
        auto ite = line.begin();
        while( *ite != ' ' ){
            status.push_back(*ite);
            ++ite;
        }
        status.push_back('?');
        sized = status.size();
        ++ite;

        string sub_line(ite,line.end()),word;
        istringstream record(sub_line);
        while(getline(record,word,',')){
            temp = vector<char>(stoi(word),'#');
            temp.push_back('.');
            to_insert.push_back(temp);
        }
    }
    status.pop_back();
    to_insert.back().pop_back();
}

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
    long long unsigned sum=0,ret;
    map<unsigned,long long unsigned> memory;
    while(getline(file,line)){
        process_line(line,status,to_insert);
        memory.clear();
        ret = count_possibilities_smart(to_insert.begin(),to_insert.end(),status.begin(), status.end(),memory);
        output << memory.size()<< " "<< ret << endl;
        sum += ret;
    }
    file.close();
    cout << "The sum of the counts is " << sum << endl;
    return 0;
}