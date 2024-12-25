#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::vector;
using std::map;
using std::set;
using std::shared_ptr;
using std::make_shared;

struct Computer{
    Computer() = delete;
    Computer(string l):label(l){}
    string label;
    set<string> connected_to;
};

struct Clique{
    Clique(const Clique& clique):labels{clique.labels},mutually_connected(clique.mutually_connected){}
    Clique(const shared_ptr<Computer> computer):labels{computer->label},mutually_connected(computer->connected_to){}
    set<string> labels;
    set<string> mutually_connected;
    void add_computer(shared_ptr<Computer> computer){
        labels.insert(computer->label);
        set<string> ret_set;
        std::set_intersection(mutually_connected.begin(), mutually_connected.end(), (computer->connected_to).begin(), (computer->connected_to).end(),std::inserter(ret_set,ret_set.begin()));
        mutually_connected = ret_set;
    }
    string get_label(string adding=""){
        vector<string> l;
        for(const string& label:labels){
            l.push_back(label);
        }
        if(adding.size())
            l.push_back(adding);
        std::sort(l.begin(),l.end());
        string name;
        for(int i = 0;i<l.size()-1;++i){
            name += l[i] + ',';
        }
        name += l.back();
        return name;
    }
};

void backtracking(Clique& clique,map<string,shared_ptr<Computer>>& network,set<string>& seen){
    for(const auto& label: clique.mutually_connected){
       if(clique.labels.find(label)==clique.labels.end()){
            string new_label = clique.get_label(label);
            const auto ret = seen.insert(new_label);
            if(ret.second){
                Clique new_clique(clique);
                new_clique.add_computer(network[label]);
                backtracking(new_clique, network, seen);
            }
       }
    }
}

int main(){
    std::ifstream file("input");
    string line;
    map<string,shared_ptr<Computer>> network;
    while(getline(file,line)){
        string c_1 = line.substr(0, 2);
        string c_2 = line.substr(3, 2);
        if(network.find(c_1)==network.end())
            network[c_1] = make_shared<Computer>(c_1);
        if(network.find(c_2)==network.end())
            network[c_2] = make_shared<Computer>(c_2);
        network[c_1]->connected_to.insert(c_2);
        network[c_2]->connected_to.insert(c_1);
    }
    
    // Part 1 
    int ret_1 = 0;
    set<string> lan;
    for(const auto& [label,ptr_0]:network){
        set<string> third_layer;
        for(const auto& label_1:ptr_0->connected_to){
            if(label_1 != label){
                for(const auto& label_2:network[label_1]->connected_to){
                    for(const auto& label_3:network[label_2]->connected_to){
                        const auto& ptr_3 = network[label_3];
                        if(label_3==label){
                            vector<string> l{label_2,label_1,label};
                            std::sort(l.begin(),l.end());
                            const auto ret_insert = lan.insert(l[0]+','+l[1]+','+l[2]);
                            if(ret_insert.second && (label_2[0]=='t' || label_1[0]=='t' || label[0]=='t')){
                                ++ret_1;
                            }
                        }
                    }
                }
            }
        }
    }
    cout << "Part 1: " << ret_1 << endl;

    // Part 2 
    set<string> seen;
    for(const auto& [label,ptr]:network){
        Clique clique(ptr);
        backtracking(clique, network, seen);
    }
    string ret_2 = *seen.begin();
    for(const auto& s:seen){
        if(s.size()>ret_2.size())
            ret_2 = s;
    }
    cout << "Part 2: " << ret_2 << endl;
    return 0;
}