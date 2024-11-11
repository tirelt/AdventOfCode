#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>
#include <set>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::accumulate;
using std::copy;
using std::set;

struct Group{
    Group() = delete;
    Group(bool b, unsigned s):is_broken(b),size(s){} 
    bool is_broken;
    unsigned size;
};

bool operator==(Group const& g,Group const& h){
    return g.is_broken == h.is_broken && g.size == h.size;
}

bool operator!=(Group const& g,Group const& h){
    return !operator==(g,h);
}

struct Group_Set{
    Group_Set():group_set(){}
    Group_Set(vector<Group> g):group_set(1,g){}
    vector<vector<Group>> group_set;
    bool add_groups(vector<Group> const& groups){
        bool add = true;
        for(vector<Group> const& g_l : group_set){
            add = add && (g_l != groups);
        }
        if(add)
            group_set.push_back(groups);
    }
};

Group_Set add_one(vector<Group>& groups){
    Group_Set new_groups_list;
    vector<Group> groups_temp;
    for(unsigned i=0;i<groups.size();++i){
        if(!groups[i].is_broken){
            groups_temp = groups;
            groups_temp[i].size += 1;
            new_groups_list.add_groups(groups_temp);
        }
    }
    if(groups.front().is_broken){
        groups_temp = groups;
        groups_temp.emplace(groups_temp.begin(),false,1);
        new_groups_list.add_groups(groups_temp);
    }
    if(groups.back().is_broken){
        groups_temp = groups;
        groups_temp.emplace(groups_temp.end(),false,1);
        new_groups_list.add_groups(groups_temp);
    }
    return new_groups_list;
}

Group_Set add_one_to_each(Group_Set& groups_list){
    Group_Set  new_group_list;
    Group_Set  new_group_list_temp;
    for(auto groups : groups_list.group_set){
        new_group_list_temp = add_one(groups);
        for(auto g : new_group_list_temp.group_set){
            new_group_list.add_groups(g);
        }
    }
    return new_group_list;
}

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
    unsigned ret = 1 + std::pow(damaged_group.size() + 1,status.size()-n_min);
    return ret;
}
//auto cmp = [](vector<Group> a, vector<Group> b) { return a!=b; };

int main(){
    std::ifstream file("test_input");
    std::ofstream output("output");
    string line;
    vector<char> status;
    vector<unsigned> damaged_group; 
    vector<Group> groups;
    while(getline(file,line)){
        process_line(line,status,damaged_group,groups);
        Group_Set group_set(groups);
        unsigned n_min = damaged_group.size() - 1 + accumulate(damaged_group.begin(),damaged_group.end(),0);
        for(unsigned i=0;i<status.size()-n_min;++i){
            group_set = add_one_to_each(group_set);
        }
        output << number_of_possibilities(status, damaged_group) << endl;
    }
    file.close();
    cout << "The sum of the lengths is: " << 0 << endl;
    return 0;
}