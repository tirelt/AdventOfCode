#include <fstream>
#include <iostream>
#include <string>
#include <regex>

using std::string;
using std::cout;
using std::endl;

int main(){
    std::ifstream file("input");
    string line;
    int total_1 = 0, total_2 = 0;
    while(getline(file,line)){
        std::regex pattern(R"((\d+)-(\d+),(\d+)-(\d+))");
        std::smatch matches;
        if (std::regex_search(line, matches, pattern)) {
            int num1 = std::stoi(matches[1].str());
            int num2 = std::stoi(matches[2].str());
            int num3 = std::stoi(matches[3].str());
            int num4 = std::stoi(matches[4].str());
            if ((num1 <= num3 && num2 >= num4) || (num3 <= num1 && num4 >= num2)) 
                ++total_1;
            if ((num1 <= num3 && num2 >= num3) || (num3 <= num1 && num4 >= num1)) 
                ++total_2;
        }
    }
    file.close();
    cout<< "Part 1: "<< total_1 << endl; 
    cout<< "Part 2: "<< total_2 << endl; 
    return 0;
}