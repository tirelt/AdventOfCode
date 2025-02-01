
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
using std::pair;

struct Component{
    Component(){}
    Component(const string& s):name(s){}
    string name;
    list<Component*> connected_to;    
};

const std::string& get_name_aux(const std::string& v, std::unordered_map<std::string, std::pair<std::string, int>>& name_mapping){
  if(name_mapping[v].first==""){
    name_mapping[v].first = v;
  }
  if(v != name_mapping[v].first) {
    name_mapping[v].first = get_name_aux(name_mapping[v].first, name_mapping);
  }
  return name_mapping[v].first;
}

struct Vertex{
    Vertex(const string& s):name(s){}
    string name;
    Vertex* mapped_to;
    string get_name() {
        return (this->update_mapping())->name;
    }
    Vertex* update_mapping(){
      if(this->mapped_to == nullptr)
        return this;
      else
        mapped_to = mapped_to->update_mapping();
      return mapped_to;
    }
};

struct Edge{
    Edge(Vertex* s_1,Vertex* s_2):v_1(s_1),v_2(s_2){}
    Vertex* v_1;
    Vertex* v_2;
};

void create_components(map<string,Component*>& components,vector<string>& input,vector<Edge>& edges,map<string,Vertex*>& vertices){
    std::regex pattern(R"(\w{3})"); 
    string first = "";
    for(const auto& line:input){
        first = "";
        for(sregex_iterator it(line.begin(), line.end(), pattern), end_it;it != end_it; ++it){
            if(!first.size()){
                first = it->str();
                if(components.find(first)==components.end()){
                    components[first] = new Component(first);
                    vertices[first] = new Vertex(first);
                }
            } else{
                string name = it->str();
                if(components.find(name)==components.end()){
                    components[name] = new Component(name);
                    vertices[name] = new Vertex(name);
                }
                components[first]->connected_to.push_back(components[name]);
                components[name]->connected_to.push_back(components[first]);
                edges.emplace_back(vertices[first],vertices[name]);
            }    
        }
    }
}

bool karger_algo(vector<Edge> edges, int number_vertices,vector<Edge>& to_cut,std::unordered_map<std::string, std::pair<std::string, int>> subset,map<string,Vertex*> vertices){
    static random_device dev;
    static mt19937 rng(dev());
    std::shuffle(edges.begin(), edges.end(), rng);

    //reset mapping
    for(auto& c:vertices){
      c.second->mapped_to=nullptr;
    }
    to_cut.clear();

    int i = 0;
    while(number_vertices>2){
      const auto name_1 = edges[i].v_1->get_name();
      const auto name_2 = edges[i].v_2->get_name();
      if(name_1!=name_2){
        vertices[name_2]->mapped_to=vertices[name_1];
        --number_vertices;
      }
      ++i;
    }
    for(int j=i;j<edges.size();++j){
      const auto name_1 = edges[j].v_1->get_name();
      const auto name_2 = edges[j].v_2->get_name();
      if(name_1!=name_2){
          to_cut.push_back(edges[j]);
      }
    }
    /*
    while(number_vertices>2){
      const auto&  e = edges[i];
      const auto& v_1 = get_name_aux(e.v_1.name, subset);
      const auto& v_2 = get_name_aux(e.v_2.name, subset);
      const auto& v_1_ = edges[i].v_1.get_name();
      const auto& v_2_ = edges[i].v_2.get_name();
      if(v_1!=v_2){
          subset[v_2].first = v_1;

          --number_vertices;
      }
      --i;
    }
    for(int j=0;j<=i;++j){
      const auto&  e = edges[j];
        const auto& v_1 = get_name_aux(e.v_1.name, subset);
        const auto& v_2 = get_name_aux(e.v_2.name, subset);
        if(v_1!=v_2){
            to_cut.push_back(edges[j]);
        }
    }
    */
    
}

int main(){
    std::ifstream file("input");
    string line;
    vector<string> input;
    while(getline(file,line)){
        input.push_back(line);
    }
    map<string,Component*> components;
    map<string,Vertex*> vertices;
    vector<Edge> edges;
    create_components(components,input,edges,vertices);
    int number_vertices = vertices.size();
    vector<Edge> to_cut;
    int count = 0;
    std::unordered_map<std::string, std::pair<std::string, int>> subsets;
    do{
        ++count;
        karger_algo(edges,number_vertices,to_cut,subsets,vertices);
    } while(to_cut.size()>3);

    cout << "Part 1: " << 0 << endl;

    return 0;
}