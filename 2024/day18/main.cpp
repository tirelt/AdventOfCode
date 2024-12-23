#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cmath>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::pair;
using std::list;

struct Space{
    bool blocked = false;
    bool visited = false;
    int depth = 0;
};

bool can_add(int x,int y, int depth, vector<vector<Space>>& memory){
    if(x>=0 && x < memory[0].size() && y>=0 && y < memory.size() ){
        if(!memory[y][x].blocked && !memory[y][x].visited){
            memory[y][x].visited = true;
            memory[y][x].depth = depth + 1;
            return true;
        }
    }
    return false;
}

bool reached(int x,int y, vector<vector<Space>>& memory){
    if(x>=0 && x < memory[0].size() && y>=0 && y < memory.size()){
        if(memory[y][x].visited){
            return true;
        }
    }
    return false;
}

void print(vector<vector<Space>>& memory){
    string temp = "";
    std::ofstream out("output");
    for(vector<Space>& line:memory){
        temp.clear();
        for(Space& s:line){
            if(s.blocked){
                temp.push_back('#');
            } else if(s.visited){
                temp.push_back('0');
            } else{
                temp.push_back('.');
            }
        }
        out << temp << "\n";
    }
    out.flush();
}

void breadth_search(list<pair<int,int>>& queue, vector<vector<Space>>& memory){
    const pair<int,int> head = queue.front();
    queue.pop_front();
    if(can_add(head.first+1,head.second,memory[head.second][head.first].depth,memory))
        queue.emplace_back(head.first+1,head.second);
    if(can_add(head.first-1,head.second,memory[head.second][head.first].depth,memory))
        queue.emplace_back(head.first-1,head.second);
    if(can_add(head.first,head.second+1,memory[head.second][head.first].depth,memory))
        queue.emplace_back(head.first,head.second+1);
    if(can_add(head.first,head.second-1,memory[head.second][head.first].depth,memory))
        queue.emplace_back(head.first,head.second-1);
}

int main(){
    std::ifstream file("input");
    int min_v = 99,max_v=0;
    string line,int_str;
    vector<pair<int,int>> falling_bytes;
    while(getline(file,line)){
        stringstream temp(line);
        pair<int,int> byte;
        getline(temp,int_str,',');
        byte.first = stoi(int_str);
        getline(temp,int_str,',');
        byte.second = stoi(int_str);
        falling_bytes.emplace_back(byte);
        min_v = std::min(min_v,std::min(byte.first,byte.second));
        max_v = std::max(max_v,std::max(byte.first,byte.second));
    }
    file.close();
    int n = max_v - min_v + 1;
    int first_n_byes = 1024; //1024
    vector<vector<Space>> memory(n,vector<Space>(n));
    
    //Part 1 
    for(int i=0;i<first_n_byes;++i){
        memory[falling_bytes[i].second][falling_bytes[i].first].blocked = true;
    }
    list<pair<int,int>> queue{{0,0}};
    memory[0][0].visited = true;
    while(queue.size() && !(queue.front().first == max_v && queue.front().second == max_v)){
        breadth_search(queue,memory);
    }
    cout << "Part 1: " << memory[queue.front().second][queue.front().first].depth << endl;
    
    //Part 2
    for(int i=first_n_byes;i<falling_bytes.size();++i){
        memory[falling_bytes[i].second][falling_bytes[i].first].blocked = true;
    }
    for(vector<Space>& line:memory){
        for(Space& s:line){
            s.depth=0;
            s.visited=false;
        }
    }
    queue.clear();
    queue.emplace_back(0,0);
    memory[0][0].visited = true;
    while(queue.size() && !(queue.front().first == max_v && queue.front().second == max_v)){
        breadth_search(queue,memory);
    }
    //print(memory);
    int i = falling_bytes.size()-1;
    for(;i>=0;--i){
        memory[falling_bytes[i].second][falling_bytes[i].first].blocked = false;
        if(reached(falling_bytes[i].first,falling_bytes[i].second-1,memory))
            queue.emplace_back(falling_bytes[i].first,falling_bytes[i].second-1);
        if(reached(falling_bytes[i].first,falling_bytes[i].second+1,memory))
            queue.emplace_back(falling_bytes[i].first,falling_bytes[i].second+1);
        if(reached(falling_bytes[i].first-1,falling_bytes[i].second,memory))
            queue.emplace_back(falling_bytes[i].first-1,falling_bytes[i].second);
        if(reached(falling_bytes[i].first+1,falling_bytes[i].second,memory))
            queue.emplace_back(falling_bytes[i].first+1,falling_bytes[i].second);
        while(queue.size() && !(queue.front().first == max_v && queue.front().second == max_v)){
            breadth_search(queue,memory);
        }
        //print(memory);
        if(queue.size() && queue.front().first == max_v && queue.front().second == max_v)
            break;   
    }
    cout<< "Part 2: " << falling_bytes[i].first << ',' << falling_bytes[i].second << endl;
    return 0;
}