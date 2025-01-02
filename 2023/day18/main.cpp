#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <regex>
#include <cmath>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::regex_search;
using std::smatch;
using std::min;
using std::max;
using std::vector;
using std::list;
using std::pair;
using std::set;

void print(vector<vector<char>>& plan){
    string temp;
    std::ofstream o("output");
    for(const auto& line:plan){
        temp.clear();
        for(const auto& c:line){
            temp.push_back(c);
        }
        o << temp << endl;
    }
    o.close();
}

int depth_search(pair<int,int> pos_init, vector<vector<char>>& plan){
    list<pair<int,int>> queue{pos_init};
    set<pair<int,int>> seen{pos_init};
    int number = 0;
    while(queue.size()){
        const auto [i,j] = queue.front();
        queue.pop_front();
        ++number;
        if(i+1<plan.size() && plan[i+1][j] == '.'){
            const auto ret = seen.insert({i+1,j});
            if(ret.second) queue.emplace_back(i+1,j);
        }
        if(i-1>=0 && plan[i-1][j] == '.'){
            const auto ret = seen.insert({i-1,j});
            if(ret.second) queue.emplace_back(i-1,j);
        }
        if(j+1<plan[0].size() && plan[i][j+1] == '.'){
            const auto ret = seen.insert({i,j+1});
            if(ret.second) queue.emplace_back(i,j+1);
        }
        if(j-1>=0 && plan[i][j-1] == '.'){
            const auto ret = seen.insert({i,j-1});
            if(ret.second) queue.emplace_back(i,j-1);
        }
    }
    return number;
}

template<typename T> 
struct CircularList{
    using Iterator = typename std::list<T>::iterator;
    CircularList() = default;
    std::list<T> elements;
    void push_back(const T& value){ elements.push_back(value); return; }
    size_t size(){ return elements.size(); }
    Iterator begin() { return elements.begin(); }
    Iterator end() { return elements.end(); }
    Iterator next(Iterator it);
    Iterator prev(Iterator it);
    Iterator erase(Iterator ite){ 
        auto ret = elements.erase(ite);
        if (ret == elements.end())
            ret = elements.begin();
        return ret;
    }
};

template<typename T> 
typename CircularList<T>::Iterator CircularList<T>::next(typename CircularList<T>::Iterator it){
    ++it;
    if (it == elements.end())
        it = elements.begin();
    return it;
}

template<typename T> 
typename CircularList<T>::Iterator CircularList<T>::prev(typename CircularList<T>::Iterator it){
    if (it == elements.begin())
        return --elements.end();
    else 
        return --it;
    return it;
}

template<typename T> 
typename CircularList<T>::Iterator collaspe(typename CircularList<T>::Iterator ite,CircularList<T>& instructions_2){
    auto new_ite = instructions_2.prev(ite);
    if(new_ite->first == ite->first){
        ite->second += new_ite->second;
        ite = instructions_2.erase(new_ite);
    } else if((new_ite->first == 'R' && ite->first=='L') || (new_ite->first == 'L' && ite->first=='R') || (new_ite->first == 'U' && ite->first=='D') || (new_ite->first == 'D' && ite->first=='U')){
        if(ite->second>new_ite->second){
            ite->second -= new_ite->second;
            ite = instructions_2.erase(new_ite);
        } else if(new_ite->second>ite->second){
            new_ite->second -= ite->second;
            ite = instructions_2.erase(ite);
        } else{
            ite = instructions_2.erase(new_ite);
            ite = instructions_2.erase(ite);
        }
    }
    return ite;
}

int main(){
    std::ifstream file("test_input");
    regex pattern(R"((\w)\s(\d+)\s\(#([a-fA-F0-9]+)\))");
    smatch matches;
    string line;
    list<pair<char,int>> instructions_1;
    CircularList<pair<char,long long>> instructions_2;
    int i=0,j=0,max_i=0,min_i=0,max_j=0,min_j=0;
    while(getline(file,line)){
        regex_search(line,matches,pattern);
        int steps = stoi(matches.str(2));
        char dir = matches.str(1)[0];
        switch (dir)
        {
        case 'R':
            j += steps;
            break;
        case 'D':
            i += steps;
            break;
        case 'L':
            j -= steps;
            break;
        case 'U':
            i -= steps;
            break;
        }
        min_i = min(min_i,i);
        min_j = min(min_j,j);
        max_i = max(max_i,i);
        max_j = max(max_j,j);
        instructions_1.push_back({dir,steps});
        char new_dir;
        switch (matches.str(3).back())
        {
        case '0':
            new_dir = 'R';
            break;
        case '1':
            new_dir = 'D';
            break;
        case '2':
            new_dir = 'L';
            break;
        case '3':
            new_dir = 'U';
            break;
        }
        instructions_2.push_back({new_dir,stoi(matches.str(3).substr(0,matches.str(3).size()-1), nullptr, 16)});
    }
    file.close();
    vector<vector<char>> plan(max_i-min_i+1,vector<char>(max_j-min_j+1,'.'));
    i += -min_i;
    j += -min_j;
    max_i=0,min_i=0,max_j=0,min_j=0;
    int ret_1 = 0;
    for(auto& [dir,steps]:instructions_1){
        switch (dir)
        {
        case 'R':
            while(steps){
                plan[i][j] = '#';
                ++j;
                --steps;
                ++ret_1;
            }
            break;
        case 'D':
            while(steps){
                plan[i][j] = '#';
                ++i;
                --steps;
                ++ret_1;
            }
            break;
        case 'L':
            while(steps){
                plan[i][j] = '#';
                --j;
                --steps;
                ++ret_1;
            }
            break;
        case 'U':
            while(steps){
                plan[i][j] = '#';
                --i;
                --steps;
                ++ret_1;
            }
            break;
        }
    }
    print(plan);
    for(int k=0;k<plan[0].size();++k){
        if(plan[0][k]=='#' && plan[1][k]=='.'){
            i = 1;
            j = k;
            break;
        }
    }
    ret_1 += depth_search({i,j}, plan);
    cout << "Part 1  " << ret_1 << endl;
    
    int index = 0,max_ite = 1000,prev_index,next_index;
    long long ret_2 = 0;
    set<string> recognized_pattern{"URD","RDL","DLU","LUR"};
    auto ite = instructions_2.begin();
    while(instructions_2.size()>2 && index<max_ite){
        auto prev_ite = instructions_2.prev(ite);
        auto next_ite = instructions_2.next(ite);
        string p;
        p.push_back(prev_ite->first);
        p.push_back(ite->first);
        p.push_back(next_ite->first);
        if(recognized_pattern.find(p)!=recognized_pattern.end()){
            if(prev_ite->second>next_ite->second){
                prev_ite->second -= next_ite->second;
                ret_2 += next_ite->second*ite->second;
                ite = instructions_2.erase(next_ite);
                ite = instructions_2.prev(ite);
            } else if (next_ite->second>prev_ite->second){
                next_ite->second -= prev_ite->second;
                ret_2 += prev_ite->second*ite->second;
                ite = instructions_2.erase(prev_ite);
            } else{
                ret_2 += prev_ite->second*ite->second;
                ite = instructions_2.erase(prev_ite);
                ite = instructions_2.prev(instructions_2.erase(instructions_2.next(ite)));
            }
            ite = collaspe(ite,instructions_2);
            ite = collaspe(instructions_2.next(ite),instructions_2);
        } else{
            ++ite;
        }
        ++index;
    }
    return 0;
}