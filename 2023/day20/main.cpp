
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <regex>
#include <functional>
#include <vector>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::sregex_iterator;
using std::smatch;
using std::regex_search;
using std::list;
using std::shared_ptr;
using std::make_shared;
using std::pair;
using std::map;
using std::vector;

//enum class Pulse {high,low};

struct Pulse{
    enum State {high,low};
    Pulse(const string f,const State s,const string t):from(f),state(s),to(t){}
    State state;
    string from;
    string to;
};

struct Module{
    Module(const string n):name(n){}
    list<string> modules;
    static list<Pulse> queue;
    virtual void send_pulse(const Pulse){}
    virtual bool is_conjuction(){ return false;}
    string name;
};

list<Pulse> Module::queue;

struct Broadcaster:Module{
    using Module::Module;
    void send_pulse(const Pulse p) override {
        for(const auto m:modules){
            queue.emplace_back(name,p.state,m);
        }
    }
};

struct Flip_flop:Module{
    using Module::Module;
    bool is_on = false;
    void send_pulse(const Pulse p) override {
        if(p.state==Pulse::low){
            if(is_on){
                for(const auto m:modules) queue.emplace_back(name,Pulse::low,m);
            } else {
                for(const auto m:modules) queue.emplace_back(name,Pulse::high,m);
            }
            is_on = !is_on;
        }
    }
};

struct Conjunction:Module{
    using Module::Module;
    map<string,Pulse::State> inputs;
    bool is_conjuction() override { return true;}
    void send_pulse(const Pulse p) override {
        inputs[p.from] = p.state;
        bool send_low = true;
        for(const auto [input,r_p]:inputs){
            if(r_p==Pulse::low){
                send_low = false;
                break;
            }
        }
        Pulse::State p_to_send = send_low?Pulse::low:Pulse::high;
        for(const auto m:modules){
            queue.emplace_back(name,p_to_send,m);
        }
    }
};

int main(){
    std::ifstream file("input");
    std::regex pattern(R"((\S+) -> (.+))"); 
    std::regex sub_pattern(R"(\b\w+\b)"); 
    std::smatch matches;
    string line;
    vector<vector<string>> modules_str;
    while(getline(file,line) && line.size()){
        vector<string> temp;
        for(sregex_iterator it(line.begin(), line.end(), pattern),end;it!=end;++it){
            temp.push_back(it->str(1));
            string sub_str = it->str(2);
            for(sregex_iterator sub_it(sub_str.begin(), sub_str.end(), sub_pattern),sub_end;sub_it!=sub_end;++sub_it){
                temp.push_back(sub_it->str());
            }
        }
        modules_str.push_back(temp);
    }
    map<string,shared_ptr<Module>> modules_map;
    for(const auto m_str:modules_str){
        if(m_str[0][0] == '&'){
            modules_map[m_str[0].substr(1)] = make_shared<Conjunction>(m_str[0].substr(1));
        } else if(m_str[0][0] == '%'){
            modules_map[m_str[0].substr(1)] = make_shared<Flip_flop>(m_str[0].substr(1));
        } else {
            modules_map[m_str[0]] = make_shared<Broadcaster>(m_str[0]);
        }
    }
    
    for(const auto m_str:modules_str){
        for(int i=1;i<m_str.size();++i){
            string m(m_str[0]);
            if(m[0]=='%'||m[0]=='&') m = m.substr(1);
            modules_map.at(m)->modules.push_back(m_str[i]);
            if(modules_map.find(m_str[i])!=modules_map.end()){
                if(modules_map.at(m_str[i])->is_conjuction()){
                    auto temp = std::dynamic_pointer_cast<Conjunction>(modules_map.at(m_str[i]));
                    temp->inputs[m] = Pulse::low;
                }
            }
        }
    }
    long long total_lows = 0, total_highs = 0;
    for(int i=0;i<1000;++i){
        Module::queue.emplace_back("button",Pulse::low,"broadcaster");
        int lows = 0, highs = 0;
        while(Module::queue.size()){
            const auto p =  Module::queue.front();
            if(p.state==Pulse::high) ++highs; else ++lows;
            cout << p.from << " " << (p.state==Pulse::high?"high":"low") << " " << p.to <<endl;
            Module::queue.pop_front();
            if(modules_map.find(p.to)!=modules_map.end()){
                modules_map.at(p.to)->send_pulse(p);
            }
        }
        total_lows += lows;
        total_highs += highs;
        //cout << "low: " << lows << " " <<"high: " << highs << endl;
    }
    long long ret_1 = total_lows * total_highs;
    cout << "Part 1: " << ret_1 << endl;
    cout << "Part 2: " << 0 << endl;
    return 0;
}