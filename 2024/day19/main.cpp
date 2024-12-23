#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <regex>

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::list;
using std::vector;
using std::regex;
using std::sregex_iterator;
using std::map;

pair<bool,long long> can_form(const string& pattern,const list<string>& towels,map<string,pair<bool,long long>>& memo,bool lazy){
    if(!pattern.size()){
        memo[pattern] = {true,1};
        return memo[pattern];
    }
    if(memo.find(pattern)!=memo.end()){
        return memo[pattern];
    }
    pair<bool,long long> ret = {false,0};
    memo.insert({pattern,{false,0}});
    for(const string& towel:towels){
        if(pattern.size() >= towel.size() && pattern.substr(0,towel.size()) == towel){
            ret = can_form(pattern.substr(towel.size()),towels,memo,lazy);
            if(ret.first){
                memo[pattern].first = true;
                memo[pattern].second += ret.second;
                if(lazy)
                    break;
            }
        }
    }
    return memo[pattern];
}
int main(){
    std::ifstream file("input");
    string line;
    list<string> patterns;
    list<string> towels;
    string pattern_regex(R"((\w+))");
    regex r(pattern_regex);
    getline(file,line);
    for(sregex_iterator ite(line.begin(), line.end(), r), end_it;ite != end_it; ++ite){
        towels.push_back(ite->str());
    }
    getline(file,line); //empty
    while(getline(file,line)){
        patterns.push_back(line);
    }
    map<string,pair<bool,long long>> memo;
    int ret_1 = 0;
    for(const string& pattern:patterns){
        if(can_form(pattern,towels,memo,true).first)
            ++ret_1;
    }
    cout<< "Part 1: " << ret_1 << endl;
    
    long long ret_2 = 0;
    memo.clear();
    for(const string& pattern:patterns){
        list<string> usable_towels(towels);
        if(can_form(pattern,usable_towels,memo,false).first){
            ret_2 += memo[pattern].second;
        }
    }
    cout<< "Part 2: " << ret_2 << endl;
    return 0;
}