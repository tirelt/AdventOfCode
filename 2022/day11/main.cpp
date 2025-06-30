#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::stoi;

struct Monkey {
    int number;
    vector<int> items;
    char operation; // '+' or '*'
    string operand; // can be a number or 'old'
    int test_divisor;
    Monkey* true_monkey;
    Monkey* false_monkey;
    int inspections = 0;
    Monkey(int num, vector<int>& it, char op, string opd, int test, Monkey* true_m, Monkey* false_m)
        : number(num), items(it), operation(op), operand(opd), test_divisor(test),
          true_monkey(true_m), false_monkey(false_m) {}
    void process(){
        for (int item : items) {
            ++inspections;
            int operand_int= (operand == "old") ? item : stoi(operand);
            int new_value = (operation=='*'?item * operand_int: item + operand_int)/ 3;
            if (new_value % test_divisor == 0) {
                true_monkey->items.push_back(new_value);
            } else {
                false_monkey->items.push_back(new_value);
            }
        }
        items.clear(); 
    }
};
    
int main(){
    std::ifstream file("input");

    std::stringstream buffer;
    buffer << file.rdbuf();
    string input = buffer.str();
    
    map<int, Monkey*> monkeys;
    map<int, std::pair<int, int>> monkey_mapping;
    // Thank you AI overlords for the regex
    std::regex pattern(R"(Monkey (\d+):\n\s*Starting items: ([\d,\s]+)\n\s*Operation: new = old ([*+]) (\d+|\w+)\n\s*Test: divisible by (\d+)\n\s*If true: throw to monkey (\d+)\n\s*If false: throw to monkey (\d+))");

    auto matches_begin = std::sregex_iterator(input.begin(), input.end(), pattern);
    auto matches_end = std::sregex_iterator();
    for (auto it = matches_begin; it != matches_end; ++it) {
        std::smatch match = *it;
        int monkey_number = stoi(match[1].str());
        string items_str = match[2].str();
        string operator_str = match[3].str();
        string operand = match[4].str();
        int test_divisor = stoi(match[5].str());
        int true_monkey = stoi(match[6].str());
        int false_monkey = stoi(match[7].str());
        vector<int> starting_items;

        monkey_mapping[monkey_number] = {true_monkey, false_monkey};
        std::stringstream ss(items_str);
        string item;
        while (std::getline(ss, item, ','))
            starting_items.push_back(std::stoi(item));
        monkeys[monkey_number] = new Monkey(monkey_number, starting_items,operator_str[0], operand, test_divisor, nullptr, nullptr);
        
    }
    file.close();    

    for(auto& pair : monkey_mapping) {
        int monkey_number = pair.first;
        monkeys[monkey_number]->true_monkey = monkeys[pair.second.first];
        monkeys[monkey_number]->false_monkey = monkeys[pair.second.second];
    }
    for(int round = 0; round < 20; ++round) {
        for (auto& pair : monkeys)
            pair.second->process();
    }
    int max_inspections = 0;
    int second_max_inspections = 0;
    for (auto& pair : monkeys) {
        int inspections = pair.second->inspections;
        if (inspections > max_inspections) {
            second_max_inspections = max_inspections;
            max_inspections = inspections;
        } else if (inspections > second_max_inspections) {
            second_max_inspections = inspections;
        }
    }
    cout<< "Part 1: " << max_inspections * second_max_inspections << endl; 
    cout<< "Part 2: "<< 0 << endl; 
    return 0;
}