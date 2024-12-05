#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <utility>
#include <map>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::set;
using std::pair;
using std::vector;

bool check_order_and_find_mid(map<int,pair<set<int>,set<int>>>& ordering, vector<int>& values, int& mid){
    map<int,int> smaller_then;
    for(const int& el:values){
        for(const int& v:values){
            if(ordering[v].first.find(el)!=ordering[v].first.end()){
                ++smaller_then[el];
            }
        }
    }
    for(const pair<int,int>& p:smaller_then){
        if(p.second==values.size()/2){
            mid = p.first;
        }
    }
    int el;
    for(int i=0; i<values.size();++i){
        el = values[i];
        if( smaller_then[el] != values.size() -(i+1)){
            return false;
        }
    }
    return true; 
}

int main(){
    std::ifstream file("input");
    string line;
    unsigned ret_1 = 0;
    unsigned ret_2 = 0;
    bool seen_space=false;
    map<int,pair<set<int>,set<int>>> ordering;
    string left;
    string right;
    int left_int;
    int right_int;
    vector<int> values;
    int mid;
    while(getline(file,line)){
        if(seen_space){
            std::istringstream stream(line);
            values.clear();
            while(getline(stream,left,',')){
                values.push_back(stoi(left));
            }
            if(check_order_and_find_mid(ordering, values,mid)){
                ret_1 += mid;
            } else{
                ret_2 += mid;
            }
        }
        if(line=="")
            seen_space = true;
        if(!seen_space){
            std::istringstream stream(line);
            getline(stream,left,'|'),getline(stream,right,'|');
            left_int = stoi(left);
            right_int = stoi(right);
            ordering[left_int].second.insert(right_int);
            ordering[right_int].first.insert(left_int);
        }
    }
    
    cout<< "Part 1: " << ret_1 << endl;  
    cout<< "Part 2: " << ret_2 << endl;  
    return 0;
}