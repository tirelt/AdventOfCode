
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>
#include <list>
#include <random>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::regex;
using std::map;
using std::set;
using std::sregex_iterator;
using std::list;

using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::advance;

struct Component{
    Component(){}
    Component(const string& s):name(s){}
    string name;
    vector<Component*> connected_to;
    vector<string> edges;
    
};

void karger_algo(map<string,Component>& components){
    random_device rd;
    int index_1,index_2;
    mt19937 gen(rd());
    while(components.size()>2){
        uniform_int_distribution<> distr(0,components.size()-1);
        index_1 = distr(gen);
        index_2 = distr(gen);
        while(index_1==index_2){
            index_2 = distr(gen);
        }
        auto it = components.begin();
        advance(it,index_1);
        Component node_1 = it->second;

        auto it = components.begin();
        advance(it,index_2);
        Component node_2 = it->second;

        
        for(const auto c:node_1.connected_to){
            for(const auto v:c->connected_to){
                if(v->name==node_1.name){
                    v = ;
                }
            }
        }
        
    }
}

int main(){
    std::ifstream file("input");
    std::regex pattern(R"(\w{3})"); 
    std::smatch matches;
    string line;
    string first = "";
    map<string,Component> components;
    while(getline(file,line)){
        first = "";
        for(sregex_iterator it(line.begin(), line.end(), pattern), end_it;it != end_it; ++it){
            if(!first.size()){
                first = it->str();
                if(components.find(first)==components.end()){
                    components[first] = Component(first);
                }
            } else{
                string name = it->str();
                if(components.find(name)==components.end()){
                    components[name] = Component(name);
                }
                components[first].connected_to.push_back(&components[name]);
                components[first].edges.push_back(first+"-"+name);
                components[name].connected_to.push_back(&components[first]);
                components[name].edges.push_back(first+"-"+name);
            }
            
        }
    }
    vector<int> connections;
    for(auto const& c:components){
        connections.push_back(c.second.connected_to.size());
    }
    std::sort(connections.begin(),connections.end());
    Component head = components.begin()->second;//components["kpl"];
    set<string> seen{head.name};
    map<string,int> connection_count;
    /*
    set<string> seen{"bbb","cqs","vss"};
    for(auto const& s:seen){
        for(const auto& connected:components[s].connected_to){
            ++connection_count[connected->name];
        }
    }
*/
    std::pair<string,int> current_highest{head.name,0};
    int counter = 0;
    list<string> queue;
    do{
    for(const auto& connected:head.connected_to){
        if(seen.find(connected->name)==seen.end()){
            ++connection_count[connected->name];
            if(connection_count[connected->name]>current_highest.second){
                current_highest = {connected->name,connection_count[connected->name]};
            }
            queue.push_back(connected->name);
            
        }
    }
    /*
    current_highest = *connection_count.begin();
    for(const auto& c:connection_count){
        if(c.second>current_highest.second){
            current_highest = c;
        }
    }
    */
    /*
    if(current_highest.second==1){
        current_highest.first=queue.front();
        queue.pop_front();
    }*/
    head = components[current_highest.first];
    seen.insert(current_highest.first);
    connection_count.erase(current_highest.first);
    }while(true);
    cout << "Part 1: " << 0 << endl;
    return 0;
}