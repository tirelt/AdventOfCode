
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
using std::to_string;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::advance;

struct Edge{
    Edge(const string& s_1,const string& s_2):node_1(s_1),node_2(s_2){}
    string node_1;
    string node_2;
    string value()const{
        if(node_1>node_2)
            return node_2 + node_1;
        else 
            return node_1 + node_2;
    }
};

bool operator<(const Edge& e_1,const Edge& e_2){
    return e_1.value() < e_2.value();
}

struct Component{
    Component(){}
    Component(const string& s):nodes({s}),name(s){}
    Component(const Component *c):name(c->name),nodes(c->nodes),edges(c->edges),connected_to(c->connected_to){}
    Component(const string& n,Component* c_1,Component* c_2):name(n),nodes(c_1->nodes){
        for(auto& connected:c_1->connected_to){
            if(connected.first!=n){
                connected.second->connected_to[n] = this;
                connected.second->connected_to.erase(c_1->name);
            }
        }
        for(auto& connected:c_2->connected_to){
            if(connected.first!=n){
                connected.second->connected_to[n] = this;
                connected.second->connected_to.erase(c_2->name);
            }
        }
        for(const auto& node:c_2->nodes)
            nodes.insert(node);
        for(const auto& c:c_1->connected_to)
            if(c.first != n && c.first != c_2->name) connected_to[c.first]=c.second;
        for(const auto& c:c_2->connected_to)
            if(c.first != n && c.first != c_1->name) connected_to[c.first]=c.second;
        for(const auto& edge:c_1->edges){
            if(nodes.find(edge.node_1)==nodes.end()||nodes.find(edge.node_2)==nodes.end())
                edges.insert(edge);
        }
        for(const auto& edge:c_2->edges){
            if(nodes.find(edge.node_1)==nodes.end()||nodes.find(edge.node_2)==nodes.end())
                edges.insert(edge);
        }
    }
    string name;
    set<string> nodes;
    set<Edge> edges;
    map<string,Component*> connected_to;    
};

bool isNumber(const std::string& str) {
    for (char const &c : str) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

void karger_algo(map<string,Component*>& components,mt19937& gen){
    Component *node_1,*node_2;
    int index;
    
    int counter = 0;
    while(components.size()>2){
        {
            uniform_int_distribution<> distr(0,components.size()-1);
            index = distr(gen);
            auto it = components.begin();
            advance(it, index);
            node_1 = it->second;
            components.erase(it);
        }
        {
            uniform_int_distribution<> distr(0,components.size()-1);
            index = distr(gen);
            auto it = components.begin();
            advance(it, index);
            node_2 = it->second;
            components.erase(it);
        };
        string new_name = to_string(counter++);
        components[new_name] = new Component(new_name,node_1,node_2);
        delete node_1;
        delete node_2;
    }
}

void create_components(map<string,Component*>& components,vector<string>& input, std::regex& pattern){
    string first = "";
    for(const auto& line:input){
        first = "";
        for(sregex_iterator it(line.begin(), line.end(), pattern), end_it;it != end_it; ++it){
            if(!first.size()){
                first = it->str();
                if(components.find(first)==components.end()){
                    components[first] = new Component(first);
                }
            } else{
                string name = it->str();
                if(components.find(name)==components.end()){
                    components[name] = new Component(name);
                }
                Edge e(first,name);
                components[first]->connected_to[name]=components[name];
                components[first]->edges.insert(e);
                components[name]->connected_to[first]=components[first];
                components[name]->edges.insert(e);
            }    
        }
    }
}
int main(){
    std::ifstream file("input");
    std::regex pattern(R"(\w{3})"); 
    std::smatch matches;
    string line;
    vector<string> input;
    while(getline(file,line)){
        input.push_back(line);
    }
    int target = 11;
    int counter = 0;
    map<string,Component*> components_saved;
    random_device rd;
    mt19937 gen(rd());
    while(target>10){
        map<string,Component*> components;
        create_components(components,input,pattern);
        karger_algo(components,gen);
        target = components.begin()->second->edges.size();
        cout << "Section " << counter++ <<  ": " << target << endl;
        if(target<=10){
            components_saved = components;
        } else {
            for(auto& c:components){
                delete c.second;
            }
        }
    }
    cout << "Part 1: " << 0 << endl;

    return 0;
}