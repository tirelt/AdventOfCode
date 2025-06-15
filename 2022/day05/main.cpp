#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;

int main(){
    std::ifstream file("input");
    string line;
    bool instructions = false;
    map<int, vector<char>> stacks;
    std::regex pattern(R"(move (\d+) from (\d+) to (\d+))");
    std::smatch matches;
    while(getline(file,line)){
        if(line.empty()){
            instructions = true;
            for(auto& [stack_num, stack] : stacks)
                std::reverse(stack.begin(), stack.end());
            continue;
        }
        if(!instructions){
            for(int i = 1; i < line.size(); i += 4) {
                if(line[i] != ' ' && !isdigit(line[i]))
                    stacks[(i / 4) + 1].push_back(line[i]);
            }
        } else {
            if (std::regex_match(line, matches, pattern)) {
                int quantity = std::stoi(matches[1].str());
                int from = std::stoi(matches[2].str());
                int to = std::stoi(matches[3].str());
                if (stacks.find(from) != stacks.end() && stacks.find(to) != stacks.end()) {
                    vector<char>& from_stack = stacks[from];
                    vector<char>& to_stack = stacks[to];
                    int max_quantity = std::min(quantity,int(from_stack.size()));
                    for (int i = 0; i < max_quantity; ++i) {
                        to_stack.push_back(from_stack.back());
                        from_stack.pop_back();
                    }
                }
            }
        }
    }
    file.close();
    string res_1;
    for(const auto& [stack_num, stack] : stacks) {
        if(!stack.empty())
            res_1 += stack.back();
    }
    cout<< "Part 1: "<< res_1 << endl; 
    cout<< "Part 2: "<< "" << endl; 
    return 0;
}