#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <regex>
#include <cmath>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::shared_ptr;
using std::make_shared;
using std::regex;
using std::smatch;
using std::regex_search;
using std::pow;

using gate_type = bool (*)(bool, bool);

enum FunctionID {AND_GATE, OR_GATE, NOT_GATE, NUM_FUNCTIONS};

bool and_gate(bool a, bool b){
    return a && b;
}
bool or_gate(bool a, bool b){
    return a || b;
}
bool xor_gate(bool a, bool b){
    return a ^ b;
}

struct Gate;

struct Cable {
    Cable(string l):label(l),is_set(false),value(0){}
    string label;
    bool value;
    bool is_set;
    vector<shared_ptr<Gate>> gates;
    void set_value(bool);
};

struct Gate{
    Gate(shared_ptr<Cable>& l,shared_ptr<Cable>&  r,shared_ptr<Cable>& o,string gate_str)
            :left_cable(l),right_cable(r),output(o),func(func_map[gate_str]){
    }
    shared_ptr<Cable> left_cable;
    shared_ptr<Cable> right_cable;
    shared_ptr<Cable> output;
    gate_type func;
    void run(){
        if(!output->is_set && left_cable->is_set && right_cable->is_set){
            output->set_value(func(left_cable->value,right_cable->value));
        }
    }
    static map<string,gate_type> func_map;
    static void initialize_func_map(){ 
        func_map["AND"] = and_gate; 
        func_map["OR"] = or_gate; 
        func_map["XOR"] = xor_gate;
    }
};

void Cable::set_value(bool v){
    value = v;
    is_set = true;
    for(shared_ptr<Gate>& gate:gates){
        gate->run();
    }
}

void add_gate_to_cables(shared_ptr<Gate> gate){
    gate->left_cable->gates.push_back(gate);
    gate->right_cable->gates.push_back(gate);
    gate->run(); 
}
map<string, gate_type> Gate::func_map;

int main(){
    Gate::initialize_func_map();
    std::ifstream file("input");
    string line;
    map<string,shared_ptr<Cable>> network;
    string label;

    while(getline(file,line)){
        if(!line.size()) break;
        label = line.substr(0,3);
        network[label] = make_shared<Cable>(line);
        network[label]->set_value(stoi(line.substr(5,1)));
    }
    regex pattern(R"((\w+)\s+(\w+)\s+(\w+)\s+->\s+(\w+))");
    smatch matches;
    while(getline(file,line)){
        regex_search(line, matches, pattern);
        if(network.find(matches.str(1))==network.end()) network[matches.str(1)] = make_shared<Cable>(matches.str(1));
        if(network.find(matches.str(3))==network.end()) network[matches.str(3)] = make_shared<Cable>(matches.str(3));
        if(network.find(matches.str(4))==network.end()) network[matches.str(4)] = make_shared<Cable>(matches.str(4));
        shared_ptr<Gate> gate = make_shared<Gate>(network[matches.str(1)],network[matches.str(3)],network[matches.str(4)],matches.str(2));
        add_gate_to_cables(gate);
    }
    long long ret_1 = 0;
    for(const auto& [label,ptr]:network){
        if(label[0]=='z') ret_1 += ptr->value*pow(2,stoi(label.substr(1)));
    }
    cout << "Part 1: " << ret_1 << endl;
    cout << "Part 2: " << 0 << endl;
    return 0;
}