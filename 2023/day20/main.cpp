
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
using std::pair;
using std::map;
using std::vector;

enum class Pulse {high,low};

struct Module{
    list<shared_ptr<Module>> modules;
    static list<pair<shared_ptr<Module>,Pulse>> queue;
    virtual void send_pulse(const Pulse, const string){}
};

struct Broadcaster:Module{
    void send_pulse(const Pulse p, const string s = "") override {
        for(const auto m:modules){
            queue.push_back({m,p});
        }
    }
};

struct Flip_flop:Module{
    bool is_on = false;
    void send_pulse(const Pulse p, const string s = "") override {
        if(p==Pulse::low){
            if(is_on){
                for(const auto m:modules) queue.push_back({m,Pulse::low});
            } else {
                for(const auto m:modules) queue.push_back({m,Pulse::high});
            }
            is_on = !is_on;
        }
    }
};

struct Conjunction:Module{
    map<string,Pulse> inputs;
    void send_pulse(const Pulse p, const string from) override {
        inputs[from] = p;
        bool send_low = true;
        for(const auto [input,r_p]:inputs){
            if(r_p==Pulse::low){
                send_low = false;
                break;
            }
        }
        Pulse p_to_send = send_low?Pulse::low:Pulse::high;
        for(const auto m:modules){
            queue.push_back({m,p_to_send});
        }
    }
};

list<pair<shared_ptr<Module>,Pulse>> Module::queue;

int main(){
    std::ifstream file("test_input");
    std::regex pattern(R"((\S+) -> (.+))"); 
    std::regex sub_pattern(R"(\b\w+\b)"); 
    std::smatch matches;
    string line;
    while(getline(file,line) && line.size()){
        vector<std::string> temp;
        for(sregex_iterator it(line.begin(), line.end(), pattern),end;it!=end;++it){
            temp.push_back(it->str(1));
            string sub_str = it->str(2);
            for(sregex_iterator sub_it(sub_str.begin(), sub_str.end(), sub_pattern),sub_end;sub_it!=sub_end;++sub_it){
                temp.push_back(sub_it->str());
            }
        }
    }
    cout << "Part 1: " << 0 << endl;
    cout << "Part 2: " << 0 << endl;
    return 0;
}