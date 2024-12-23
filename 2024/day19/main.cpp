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

bool can_form(const string& pattern,const list<string>& towels,map<string,bool>& memo){
    if(!pattern.size()){
        return true;
    }
    if(memo.find(pattern)!=memo.end()){
        return memo[pattern];
    }
    bool ret =false;
    memo.insert({pattern,false});
    for(const string& towel:towels){
        if(pattern.size() >= towel.size() && pattern.substr(0,towel.size()) == towel){
            ret = can_form(pattern.substr(towel.size()),towels,memo);
            if(ret){
                memo[pattern] = true;
                break;
            }
        }
    }
    return ret;
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
    map<string,bool> memo;
    int ret_1 = 0;
    for(const string& pattern:patterns){
        if(can_form(pattern,towels,memo))
            ++ret_1;
    }
    cout<< "Part 1: " << ret_1 << endl;
    return 0;
}