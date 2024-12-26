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

using gate_type = bool (*)(bool, bool);

enum FunctionID {AND_GATE, OR_GATE, NOT_GATE, NUM_FUNCTIONS};

bool and_gate(bool a, bool b){
    return a && b;
}
bool or_gate(bool a, bool b){
    return a && b;
}
bool xor_gate(bool a, bool b){
    return a ^ b;
}

struct Gate;

struct Cable {
    Cable(string l):label(l){}
    string label;
    bool value;
    bool is_set;
    vector<shared_ptr<Gate>> gates;
    void set_value(bool);
};

struct Gate{
    Gate(shared_ptr<Cable>& l,shared_ptr<Cable>&  r,shared_ptr<Cable>& o,string gate_str)
            :left_cable(l),right_cable(r),output(o),func(func_map[gate_str]){
        this->run();  
    }
    shared_ptr<Cable> left_cable;
    shared_ptr<Cable> right_cable;
    shared_ptr<Cable> output;
    gate_type func;
    void run(){
    shared_ptr<Cable> output;
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

map<string, gate_type> Gate::func_map;

int main(){
    std::ifstream file("input");
    string line;
    Gate::initialize_func_map();
    cout << "Part 1: " << 0 << endl;
    cout << "Part 2: " << 0 << endl;
    return 0;
}