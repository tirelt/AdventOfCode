#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>
#include <unordered_set>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::accumulate;
using std::copy;
using std::unordered_set;

struct Group{
    Group() = delete;
    Group(bool b, unsigned s):is_broken(b),size(s){}
    bool is_broken;
    unsigned size;
};
/*
unordered_set<vector<Group>> add_one(vector<Group>& groups){
    unordered_set<vector<Group>> new_groups_list;
    vector<Group> groups_temp;
    for(unsigned i=0;i<groups.size();++i){
        if(!groups[i].is_broken){
            groups_temp = groups;
            groups_temp[i].size += 1;
            new_groups_list.insert(groups_temp);
        }
    }
    if(groups.front().is_broken){
        groups_temp = groups;
        groups_temp.emplace(groups_temp.begin(),false,1);
        new_groups_list.insert(groups_temp);
    }
    if(groups.back().is_broken){
        groups_temp = groups;
        groups_temp.emplace(groups_temp.end(),false,1);
        new_groups_list.insert(groups_temp);
    }
    return new_groups_list;
}

unordered_set<vector<Group>> add_one_to_each(unordered_set<vector<Group>>& groups_list){
    unordered_set<vector<Group>>  new_group_list;
    unordered_set<vector<Group>>  new_group_list_temp;
    for(auto groups : groups_list){
        new_group_list_temp = add_one(groups);
        for(auto g : new_group_list_temp){
            new_group_list.insert(g);
        }
    }
    return new_group_list;
}
*/
void process_line(string& line, vector<char>& status, vector<unsigned>& damaged_group,vector<Group>& groups){
    bool has_seen_separator = false;
    status.clear();
    damaged_group.clear();
    groups.clear();
    auto ite = line.begin();
    while( *ite != ' ' ){
        status.push_back(*ite);
        ++ite;
    }
    ++ite;
    string sub_line(ite,line.end()),word;
    
    istringstream record(sub_line);
    while(getline(record,word,',')){
        damaged_group.push_back(stoi(word));
        groups.emplace_back(true,damaged_group.back());
        groups.emplace_back(false,1);
    }
    groups.pop_back();
}

unsigned number_of_possibilities(vector<char>& status, vector<unsigned>& damaged_group){
    unsigned n_min = damaged_group.size() - 1 + accumulate(damaged_group.begin(),damaged_group.end(),0);
    unsigned ret = 1 + (damaged_group.size() + 1)*(status.size()-n_min);
    return ret;
}

int main(){
    std::ifstream file("test_input");
    std::ofstream output("output");
    string line;
    vector<char> status;
    vector<unsigned> damaged_group; 
    vector<Group> groups;
    while(getline(file,line)){
        process_line(line,status,damaged_group,groups);
        //unordered_set<vector<Group>> groups_list{groups};
        output << number_of_possibilities(status, damaged_group) << endl;
    }
    file.close();
    cout << "The sum of the lengths is: " << 0 << endl;
    return 0;
}