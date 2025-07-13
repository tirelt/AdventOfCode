#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <map>
#include <list>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stoi;
using std::map;
using std::list;
using std::tuple;
using std::make_tuple;
using std::get;
using std::pair;

struct Valve {
    Valve(const string& n) : name(n) {}
    string name;
    int flow_rate;
    vector<string> tunnels;
    map<string, int> distances; 
    int number;
};

vector<string> convert_states(const int state, const map<string,Valve*>& valves) {
    vector<string> res;
    for( const auto& [n,v] : valves )
        if( state & (1<<v->number))
            res.push_back(n);
}

int update_memory(const map<string,Valve*>& valves, int rate,int released, tuple<string,int,int> name_states_time,map<tuple<string,int,int>,int>& memory) {
    if (memory.find(name_states_time) != memory.end())
        return memory.at(name_states_time);
    string name = get<0>(name_states_time);
    int states = get<1>(name_states_time);
    int time = get<2>(name_states_time);
    auto temp = convert_states(states,valves);
    if (!time) {
        memory[name_states_time] = released;
        return released;
    }
    vector<int> possible_releases{time*rate+released};
    for (const auto& [next_valve,distance] : valves.at(name)->distances) {
        if (time >= (distance+1)) {
            bool state_next_valve = states & (1<<valves.at(next_valve)->number);
            if (!state_next_valve){
                int next_states = states | (1<<valves.at(next_valve)->number);
                possible_releases.push_back(update_memory(valves, rate + valves.at(next_valve)->flow_rate, released + (distance + 1) * rate, {next_valve,next_states,time-(distance+1)}, memory));
            }
        }
    }
    int max_released = *std::max_element(possible_releases.begin(),possible_releases.end());
    memory[name_states_time] = max_released;
    return max_released;
}

int main(){
    std::ifstream file("test_input");
    string line;
    std::regex pattern(R"(^Valve (\w+) has flow rate=(\d+); tunnels? leads? to valves? ([\w\s,]+)$)");
    map<string, Valve*> valves;
    vector<string> meaningfull_valves{"AA"};
    vector<string> other_valves;
    int count = 0;
    while(getline(file,line)){
        std::smatch match;
        if (std::regex_match(line, match, pattern)) {
            string valve_name = match[1];
            int flow_rate = std::stoi(match[2]);
            valves[valve_name] = new Valve(valve_name);
            valves[valve_name]->flow_rate = flow_rate;
            if (flow_rate){
                meaningfull_valves.push_back(valve_name);
                valves[valve_name]->number = count++;
            }
            else
                other_valves.push_back(valve_name);
            string tunnels_str = match[3];
            string tunnel;

            std::istringstream tunnel_stream(tunnels_str);
            while (std::getline(tunnel_stream, tunnel, ',')) {
                tunnel.erase(0, tunnel.find_first_not_of(" \t"));
                tunnel.erase(tunnel.find_last_not_of(" \t") + 1);
                if (!tunnel.empty()) 
                    valves[valve_name]->tunnels.push_back(tunnel);
            }
            auto test = 1;
        }
    }
    file.close();
    
    // check the distance between the meaningfullvalves
    for (const auto& valve_name : meaningfull_valves) {
        list<pair<string,int>> queue = {{valve_name,0}}; 
        map<string, int>* distances = &valves[valve_name]->distances;
        distances->insert({valve_name, 0});
        while (!queue.empty()) {
            const auto head = queue.front().first;
            const auto d = queue.front().second;
            queue.pop_front();
            for (const auto& tunnel : valves[head]->tunnels) { 
                if (distances->find(tunnel) == distances->end() || (*distances)[tunnel] > d + 1) {
                    (*distances)[tunnel] = d + 1;
                    //valves.at(tunnel)->distances[valve_name] = d + 1;
                    queue.push_back({tunnel, d + 1});
                }
            }
        }
        for (const auto& name : other_valves)
            distances->erase(name);
    }

    int states = 0;
    states = states | (1<<valves["AA"]->number);
    map<std::tuple<string,int,int>,int> memory;
    int res_1 = update_memory(valves, 0, 0, {"AA",states,30}, memory);
    
    for(auto& p : valves)
        delete p.second;
    return 0;
}