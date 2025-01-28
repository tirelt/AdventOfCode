
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::regex;
using std::map;
using std::set;
using std::sregex_iterator;

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
    Component head = components.begin()->second;
    set<string> seen{head.name};
    map<string,int> connection_count;
    std::pair<string,int> current_highest{head.name,0};
    std::pair<string,int> second_highest{head.name,0};
    do{
    for(const auto& connected:head.connected_to){
        if(seen.find(connected->name)==seen.end()){
            if(++connection_count[connected->name]>current_highest.second){
                second_highest = current_highest;
                current_highest = {connected->name,connection_count[connected->name]};

            }
        }
    }
    head = components[current_highest.first];
    seen.insert(current_highest.first);
    connection_count.erase(current_highest.first);
    }while(true);
    cout << "Part 1: " << 0 << endl;
    return 0;
}