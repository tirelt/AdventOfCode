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
    map<int, vector<char>> stacks_2;
    std::regex pattern(R"(move (\d+) from (\d+) to (\d+))");
    std::smatch matches;
    while(getline(file,line)){
        if(line.empty()){
            instructions = true;
            for(auto& [stack_num, stack] : stacks)
                std::reverse(stack.begin(), stack.end());
            stacks_2 = stacks; // Copy stacks for part 2
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

                    vector<char>& from_stack_2 = stacks_2[from];
                    vector<char>& to_stack_2 = stacks_2[to];
                    int max_quantity_2 = std::min(quantity,int(from_stack_2.size()));
                    vector<char> temp;
                    for (int i = 0; i < max_quantity_2; ++i) {
                        temp.push_back(from_stack_2.back());
                        from_stack_2.pop_back();
                    }
                    std::reverse(temp.begin(), temp.end());
                    for(const char& c : temp)
                        to_stack_2.push_back(c);
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
    string res_2;
    for(const auto& [stack_num, stack] : stacks_2) {
        if(!stack.empty())
            res_2 += stack.back();
    }
    cout<< "Part 1: " << res_1 << endl; 
    cout<< "Part 2: " << res_2 << endl; 
    return 0;
}