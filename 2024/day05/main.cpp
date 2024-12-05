

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::set;
using std::pair;
using std::vector;
using std::set_intersection;
using std::set_union;

bool check_order(map<int,pair<set<int>,set<int>>>& ordering, vector<int>& values){
    set<int> left_set;
    set<int> right_set(values.begin(),values.end());
    set<int> result;
    for(const int& el:values){
        result.clear();
        set_intersection(ordering[el].first.begin(),ordering[el].first.end(),right_set.begin(),right_set.end(),inserter(result, result.begin()));
        if(result.size()){
            return false;
        }
        result.clear();
        set_intersection(ordering[el].second.begin(),ordering[el].second.end(),left_set.begin(),left_set.end(),inserter(result, result.begin()));
        if(result.size()){
            return false;
        }
        right_set.erase(el);
        left_set.insert(el);
    }   
    return true; 
}

 vector<int> sort_values(map<int,pair<set<int>,set<int>>>& ordering, vector<int>& values){
    set<int> right_set;
    for(const int& el:values){
        for(const int& i:ordering[el].second){
            right_set.insert(i);
        }
    }
    vector<int> sorted_values;
    set<int> result;
    while(sorted_values.size()<values.size()){
        for(const int& el:values){
            result.clear();
            set_intersection(ordering[el].first.begin(),ordering[el].first.end(),right_set.begin(),right_set.end(),inserter(result, result.begin()));
            if(!result.size()){
                sorted_values.push_back(el);
                break;
            }
        }
        for(const int& el:ordering[sorted_values.back()].second){
            left_set.insert(el);
        }

    }
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
    while(getline(file,line)){
        if(seen_space){
            std::istringstream stream(line);
            values.clear();
            while(getline(stream,left,',')){
                values.push_back(stoi(left));
            }
            if(check_order(ordering, values)){
                ret_1 += values[values.size()/2];
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