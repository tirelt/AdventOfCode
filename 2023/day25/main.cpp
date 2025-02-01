#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <regex>
#include <map>
#include <random>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::regex;
using std::map;
using std::sregex_iterator;
using std::random_device;
using std::mt19937;

struct Vertex{
  Vertex(const string& s):name(s){}
  string name;
  Vertex* mapped_to;
  string get_name() {
      return (this->update_mapping())->name;
  }
  Vertex* update_mapping(){
    if(mapped_to == nullptr)
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

void create_edges_and_vertices(const string file_name,vector<Edge>& edges,map<string,Vertex*>& vertices){
  std::ifstream file(file_name);
  std::regex pattern(R"(\w{3})"); 
  string first = "";
  string line;
  while(getline(file,line)){
    first = "";
    for(sregex_iterator it(line.begin(), line.end(), pattern), end_it;it != end_it; ++it){
      if(!first.size()){
        first = it->str();
        if(vertices.find(first)==vertices.end()){
            vertices[first] = new Vertex(first);
        }
      } else{
        string name = it->str();
        if(vertices.find(name)==vertices.end()){
            vertices[name] = new Vertex(name);
        }
        edges.emplace_back(vertices[first],vertices[name]);
      }    
    }
  }
  file.close();
}

void karger_algo(vector<Edge>& edges, int number_vertices,vector<Edge>& to_cut,map<string,Vertex*>& vertices){
  //initialize random device only once, shuffle the edges at the begining instead of selecting randomly at every ite
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
    if(edges[j].v_1->get_name() != edges[j].v_2->get_name())
        to_cut.push_back(edges[j]);
  }    
}

int main(){
  map<string,Vertex*> vertices;
  vector<Edge> edges;
  create_edges_and_vertices("input",edges,vertices);

  int number_vertices = vertices.size();
  vector<Edge> to_cut;
  int count = 0;
  do{
    ++count;
    karger_algo(edges,number_vertices,to_cut,vertices);
  } while(to_cut.size()>3);

  map<string,int> counter;
  for(auto& v:vertices){
    ++counter[v.second->get_name()];
  }
  int ret_1 = 1;
  for(const auto p:counter){
    ret_1*=p.second;
  }
  cout << "Part 1: " << ret_1 << endl;

  //deallocates memory
  for(auto& [name,v_ptr]:vertices){
    delete v_ptr;
  }
  return 0;
}