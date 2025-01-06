#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <regex>
#include <functional>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::sregex_iterator;
using std::smatch;
using std::regex_search;
using std::list;
using std::map;
using std::function;
using std::vector;

struct Workflow{
    Workflow() = default;
    Workflow(const string s,const string d):name(s),default_case(d){}
    string name;
    string default_case;
    list<function<string (map<string,int>)>> rules;
};
int main(){
    std::ifstream file("input");
    regex pattern_1(R"((\w+|<|>|\d+))");
    string line;
    map<string,Workflow> workflows;
    while(getline(file,line) && line.size()){
        vector<string> args;
        for (sregex_iterator it(line.begin(), line.end(), pattern_1), end_it; it != end_it; ++it){
            args.push_back(it->str());
        }
        Workflow w(args[0],args[args.size()-1]);
        for(int i=0;i<(args.size()-2)/4;++i){
            int number = stoi(args[i*4+3]);
            string letter = args[i*4+1];
            string out = args[i*4+4];
            string ope = args[i*4+2];
            w.rules.push_back([letter,ope,number,out](map<string,int> part) -> string {
                if(ope=="<")
                    return part[letter]<number?out:"";
                else
                    return part[letter]>number?out:"";
                });
        }
        workflows[args[0]] = w;
    }
    regex pattern_2("\\{x\\=(\\d+),m\\=(\\d+),a\\=(\\d+),s\\=(\\d+)\\}");
    smatch results;
    list<map<string,int>> parts;
    while(getline(file,line)){
        regex_search(line, results, pattern_2);
        parts.push_back({{"x",stoi(results.str(1))},{"m",stoi(results.str(2))},{"a",stoi(results.str(3))},{"s",stoi(results.str(4))}});
    }
    Workflow w; 
    int ret_1 = 0;
    for(const auto& part:parts){
        string out("in");
        while( out!="R" && out!="A"){
            w = workflows[out];
            for(const auto& rule:w.rules){
                out =rule(part);
                if(out.size()) break;
            }
            if(!out.size()) 
                out = w.default_case;
        }
        if( out == "A" ){
            for(const auto& el:part){
                ret_1 += el.second;
            }
        }
    }
    return 0;
}