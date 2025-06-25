#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

using std::string;
using std::cout;
using std::endl;
using std::vector;

    
int main(){
    std::ifstream file("input");

    std::stringstream buffer;
    buffer << file.rdbuf();
    string input = buffer.str();


    
    // Thank you AI overlords for the regex
    std::regex monkey_regex(
        R"(Monkey \d+:\n\s*Starting items: ([^\n]+)\n\s*Operation: new = old\s+([*+])\s+(old|\d+)\n\s*Test: divisible by (\d+)\n\s*If true: throw to monkey (\d+)\n\s*If false: throw to monkey (\d+))",
        std::regex::multiline
    );
    auto matches_begin = std::sregex_iterator(input.begin(), input.end(), monkey_regex);
    auto matches_end = std::sregex_iterator();
    for (auto it = matches_begin; it != matches_end; ++it) {
        auto match = *it;
        auto a = 1;
    }


    file.close();    
    cout<< "Part 1: " << 0 << endl; 

    cout<< "Part 2: "<< 0 << endl; 
    return 0;
}