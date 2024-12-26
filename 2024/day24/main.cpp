#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <memory>
#include <regex>
#include <cmath>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::list;
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
    Cable(string l,shared_ptr<vector<string>>& o):label(l),is_set(false),value(0),tracking(o){}
    string label;
    bool value;
    bool is_set;
    vector<shared_ptr<Gate>> gates;
    shared_ptr<vector<string>> tracking;
    static bool track;
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
    if(track) tracking->push_back(this->label);
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
bool Cable::track;

struct Gate_description{
    Gate_description() = default;
    Gate_description(string o,pair<string,string> s):oper(o),operands(s){}
    Gate_description(const Gate_description& g):oper(g.oper),operands(g.operands){}
    pair<string,string> operands;
    string oper;   
};

struct Gate_comp{
    pair<shared_ptr<Gate_comp>,shared_ptr<Gate_comp>> operands;
    string oper;
    string label;
    int weight = 0;
};

bool comparison(Gate_comp& g_1, const Gate_comp& g_2,list<string>& errors){
    if(g_1.operands.first == nullptr || g_1.operands.second == nullptr || g_2.operands.first == nullptr || g_2.operands.second == nullptr){
        const bool ret = (g_1.oper == g_2.oper) && (g_1.operands.first == nullptr) && (g_1.operands.second == nullptr) && (g_2.operands.first == nullptr) && (g_2.operands.second == nullptr);
        return ret;
    } 
    const bool ret = g_1.oper == g_2.oper && ((comparison(*g_1.operands.first,*g_2.operands.first,errors) && comparison(*g_1.operands.second,*g_2.operands.second,errors))
                                             ||(comparison(*g_1.operands.first,*g_2.operands.second,errors) &&  comparison(*g_1.operands.second,*g_2.operands.first,errors)));
    if(!ret) errors.push_back(g_1.label);
    return ret;
}

shared_ptr<Gate_comp> expend(int depth, const string label,map<string,Gate_description>& output_map){
    Gate_comp g;
    shared_ptr<Gate_comp> g_ptr = make_shared<Gate_comp>(g);
    g_ptr -> label = label;
    if(!depth || label[0] == 'x' || label[0] == 'y'){
        g_ptr->oper = label;
        g_ptr->weight = 1;
        if(label[0] != 'x' && label[0] != 'y') ++(g_ptr->weight);
        return g_ptr;
    }
    g_ptr->oper = output_map[label].oper;
    shared_ptr<Gate_comp> temp_1 =  expend(depth-1, output_map[label].operands.first,output_map );
    shared_ptr<Gate_comp> temp_2 = expend(depth-1, output_map[label].operands.second,output_map );
    g_ptr->weight = temp_1->weight + temp_2->weight;
    if(temp_1->weight <= temp_2->weight){// || temp_1->label[0]=='x'
        g_ptr->operands.first = temp_1;
        g_ptr->operands.second = temp_2; 
    } else {
        g_ptr->operands.first = temp_2;
        g_ptr->operands.second = temp_1; 
    }
    return g_ptr;
}

string int_to_label(char type,int i){
    string ret;
    ret.push_back(type);
    if(i<10) ret.push_back('0');
    ret += std::to_string(i);
    return ret;
}

int main(){
    Gate::initialize_func_map();
    std::ifstream file("input");
    string line;
    map<string,shared_ptr<Cable>> network;
    string label;
    shared_ptr<vector<string>> tracking = make_shared<vector<string>>();

    Cable::track = false;
    while(getline(file,line)){
        if(!line.size()) break;
        label = line.substr(0,3);
        network[label] = make_shared<Cable>(line,tracking);
        network[label]->set_value(stoi(line.substr(5,1)));
    }
    map<string,Gate_description> output_map;
    regex pattern(R"((\w+)\s+(\w+)\s+(\w+)\s+->\s+(\w+))");
    smatch matches;
    //Cable::track = true;
    while(getline(file,line)){
        regex_search(line, matches, pattern);
        if(network.find(matches.str(1))==network.end()) network[matches.str(1)] = make_shared<Cable>(matches.str(1),tracking);
        if(network.find(matches.str(3))==network.end()) network[matches.str(3)] = make_shared<Cable>(matches.str(3),tracking);
        if(network.find(matches.str(4))==network.end()) network[matches.str(4)] = make_shared<Cable>(matches.str(4),tracking);
        shared_ptr<Gate> gate = make_shared<Gate>(network[matches.str(1)],network[matches.str(3)],network[matches.str(4)],matches.str(2));
        add_gate_to_cables(gate);
        output_map[matches.str(4)] = Gate_description(matches.str(2),{matches.str(1),matches.str(3)});
    }
    long long ret_1 = 0;
    int last=0;
    for(const auto& [label,ptr]:network){
        if(label[0]=='z'){
            last = stoi(label.substr(1));
            ret_1 += ptr->value*pow(2,last);
        } 
    }
    cout << "Part 1: " << ret_1 << endl;
    vector<string> part_2;
    for(int i=2;i<last;++i){
        //expected 
        Gate_comp g_1; g_1.oper = int_to_label('x',i-1);
        Gate_comp g_2; g_2.oper = int_to_label('y',i-1);
        Gate_comp g_3; g_3.operands.first = make_shared<Gate_comp>(g_1); g_3.operands.second = make_shared<Gate_comp>(g_2); g_3.oper = "AND";
        Gate_comp g_4; g_4.oper = output_map[int_to_label('z',i-1)].operands.first;
        Gate_comp g_5; g_5.oper = output_map[int_to_label('z',i-1)].operands.second;
        Gate_comp g_6; g_6.operands.first = make_shared<Gate_comp>(g_4); g_6.operands.second = make_shared<Gate_comp>(g_5); g_6.oper = "AND";
        Gate_comp g_7; g_7.operands.first = make_shared<Gate_comp>(g_3); g_7.operands.second = make_shared<Gate_comp>(g_6); g_7.oper = "OR";
        Gate_comp g_8; g_8.oper = int_to_label('x',i);
        Gate_comp g_9; g_9.oper = int_to_label('y',i);
        Gate_comp g_10; g_10.operands.first = make_shared<Gate_comp>(g_8); g_10.operands.second = make_shared<Gate_comp>(g_9); g_10.oper = "XOR";
        Gate_comp g_expected; g_expected.operands.first = make_shared<Gate_comp>(g_10); g_expected.operands.second = make_shared<Gate_comp>(g_7); g_expected.oper = "XOR";

        // real
        shared_ptr<Gate_comp> g_real_ptr = expend(3, int_to_label('z',i),output_map);
               
        //Comparison
        list<string> errors;
        bool test = comparison(*g_real_ptr,g_expected,errors);
        if(!test){ 
            part_2.push_back(errors.front());
        }
    }
    std::sort(part_2.begin(),part_2.end());
    cout << "Part 2: ";
    for(const auto& e:part_2) cout << e << ','; 
    cout << endl;
    return 0;
}
