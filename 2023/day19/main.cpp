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
using std::pair;
using std::array;

struct Rule{
    Rule(const string l,const string op,int  n,const string ou):letter(l),ope(op),number(n),out(ou){}
    Rule(const Rule& r):letter(r.letter),ope(r.ope),number(r.number),out(r.out){}
    string letter;
    string ope;
    int number;
    string out;
};

struct Workflow{
    Workflow() = default;
    Workflow(const string s,const string d):name(s),default_case(d){}
    string name;
    string default_case;
    list<function<string (map<string,int>)>> rules;
    list<Rule> rules_str;
};

using Part_range = map<string,pair<int,int>>;

bool non_empty(const Part_range& part_range){
    return (part_range.at("x").first+2) < part_range.at("x").second &&
           (part_range.at("m").first+2) < part_range.at("m").second &&
           (part_range.at("a").first+2) < part_range.at("a").second &&
           (part_range.at("s").first+2) < part_range.at("s").second;
}

long long size(const Part_range& part_range){
    long long ret = (part_range.at("x").second - part_range.at("x").first - 1);
    ret *= (part_range.at("m").second - part_range.at("m").first - 1);
    ret *= (part_range.at("a").second - part_range.at("a").first - 1);
    ret *= (part_range.at("s").second - part_range.at("s").first - 1);
    return ret;
}

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
            w.rules_str.emplace_back(letter,ope,number,out);
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
    cout << "Part 1: " <<ret_1 << endl;

    Part_range part_range = {{"x",{0,4001}},{"m",{0,4001}},{"a",{0,4001}},{"s",{0,4001}}};
    
    list<pair<Workflow,Part_range>> queue{{workflows["in"],part_range}};
    list<Part_range> accepted_ranges;
    while(queue.size()){
        auto [workflow,part_range] = queue.front();
        queue.pop_front();
        Part_range default_range(part_range);
        Part_range new_part_range(part_range);
        for(const auto& rule:workflow.rules_str){
            if(rule.ope =="<"){
                new_part_range[rule.letter].second = std::min(rule.number,new_part_range[rule.letter].second);
                default_range[rule.letter].first = std::max(rule.number-1,new_part_range[rule.letter].first);
            }
            if(rule.ope ==">"){
                new_part_range[rule.letter].first = std::max(rule.number,new_part_range[rule.letter].first);
                default_range[rule.letter].second = std::min(rule.number+1,new_part_range[rule.letter].second);
            }
            if(rule.out != "R" && non_empty(new_part_range)){
                if(rule.out == "A" )
                    accepted_ranges.push_back(new_part_range);
                else
                    queue.push_back({workflows[rule.out],new_part_range});
            }
            new_part_range = default_range;
        }
        if(workflow.default_case != "R" && non_empty(default_range)){
            if(workflow.default_case == "A" )
                accepted_ranges.push_back(default_range);
            else
                queue.push_back({workflows[workflow.default_case],default_range});
        }
    }
    long long ret_2 = 0;
    for(const auto accepted_range:accepted_ranges){
        ret_2 += size(accepted_range);
    }
    cout << "Part 2: " << ret_2 << endl;
    return 0;
}