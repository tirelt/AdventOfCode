
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>
#include <list>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::regex;
using std::map;
using std::set;
using std::sregex_iterator;
using std::list;


struct Component{
    Component(){}
    Component(const string& s):name(s){}
    string name;
    vector<Component*> connected_to;
};
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
                components[name].connected_to.push_back(&components[first]);
            }
            
        }
    }
    Component head = components["skp"];
    set<string> seen{head.name};
    map<string,int> connection_count;
    std::pair<string,int> current_highest{head.name,0};
    int counter = 0;
    list<string> queue;
    do{
    for(const auto& connected:head.connected_to){
        if(seen.find(connected->name)==seen.end()){
            ++connection_count[connected->name];
            queue.push_back(connected->name);
        }
    }
    current_highest = *connection_count.begin();
    for(const auto& c:connection_count){
        if(c.second>current_highest.second){
            current_highest = c;
        }
    }
    if(current_highest.second==1){
        current_highest.first=queue.front();
        queue.pop_front();
    }
    head = components[current_highest.first];
    seen.insert(current_highest.first);
    connection_count.erase(current_highest.first);
    }while(true);
    cout << "Part 1: " << 0 << endl;
    return 0;
}