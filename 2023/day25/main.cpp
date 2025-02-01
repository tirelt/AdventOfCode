
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

struct Component{
    Component(){}
    Component(const string& s):name(s){}
    string name;
    list<Component*> connected_to;    
};

struct Vertex{
    Vertex(const string& s):name(s){}
    string name;
    static map<string,string> name_mapping;
    string& get_name(){
        string name_1 = name;
        string name_2 = name_mapping[name];
        if(name_2 == ""){
            name_mapping[name] = name;
        } else{
            while(name_1 != name_2){
                name_1 = name_2;
                name_2 = name_mapping[name_2];
            }
            name_mapping[name] = name_2;
        }
        return name_mapping[name]; 
    }
};

map<string,string> Vertex::name_mapping;

struct Edge{
    Edge(const string& s_1,const string& s_2):v_1(s_1),v_2(s_2){}
    Vertex v_1;
    Vertex v_2;
};

void create_components(map<string,Component*>& components,vector<string>& input,vector<Edge>& edges){
    std::regex pattern(R"(\w{3})"); 
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
                components[first]->connected_to.push_back(components[name]);
                components[name]->connected_to.push_back(components[first]);
                edges.emplace_back(first,name);
            }    
        }
    }
}

bool karger_algo(vector<Edge> edges, int number_vertices,vector<Edge>& to_cut){
    static random_device dev;
    static mt19937 rng(dev());
    std::shuffle(edges.begin(), edges.end(), rng);
    Vertex::name_mapping.clear();
    int i = 0;
    while(number_vertices>2){
        if(edges[i].v_1.get_name()!=edges[i].v_2.get_name()){
            Vertex::name_mapping[edges[i].v_1.name]=edges[i].v_2.name;
            --number_vertices;
        } else{
            auto t = 1;
        }
        ++i;
    }
    to_cut.clear();
    for(int j=i;j<edges.size();++j){
        if(edges[j].v_1.get_name()!=edges[j].v_2.get_name()){
            to_cut.push_back(edges[j]);
        }
    }
}

int main(){
    std::ifstream file("test_input");
    string line;
    vector<string> input;
    while(getline(file,line)){
        input.push_back(line);
    }
    map<string,Component*> components;
    vector<Edge> edges;
    create_components(components,input,edges);
    int number_vertices = components.size();
    vector<Edge> to_cut;
    int count = 0;
    do{
        ++count;
        karger_algo(edges,number_vertices,to_cut);
    } while(to_cut.size()>3);

    cout << "Part 1: " << 0 << endl;

    return 0;
}