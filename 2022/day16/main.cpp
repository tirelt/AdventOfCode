#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <map>
#include <list>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stoi;
using std::map;
using std::list;

struct Valve {
    Valve(const string& n) : name(n) {}
    string name;
    int flow_rate;
    vector<string> tunnels;
};

int main(){
    std::ifstream file("input");
    string line;
    std::regex pattern(R"(^Valve (\w+) has flow rate=(\d+); tunnels lead to valves? ([\w\s,]+)$)");
    map<string, Valve*> valves;
    map<string, bool> meaningful_valves = {{"AA", false}};
    while(getline(file,line)){
        std::smatch match;
        if (std::regex_match(line, match, pattern)) {
            string valve_name = match[1];
            int flow_rate = std::stoi(match[2]);
            if (flow_rate)
                meaningful_valves[valve_name] = false;
            string tunnels_str = match[3];
            string tunnel;
            valves[valve_name] = new Valve(valve_name);

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
    
   list<string> queue = {"AA"}; 
    map<string, bool> visited;
    while (!queue.empty()) {
        string head = queue.front();
        queue.pop_front();
        visited[head] = true;
        
    }
    for(auto& pair : valves)
        delete pair.second;
    return 0;
}