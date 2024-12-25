#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <list>


using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::list;
using std::vector;
using std::map;
using std::set;

string create_name(const char& c,const char& b){
    string ret;
    ret.push_back(c);
    ret.push_back(b);
    return ret;
}

void find_aux(const pair<int,int>& head, int d_i, int d_j,list<pair<int,int>>& queue, const char& from, map<string,string>& memo, const vector<vector<char>>& keyboard){
    string current_name = create_name(from,keyboard[head.first][head.second]);
    if(head.first+d_i>=0&&head.first+d_i<keyboard.size()&&head.second+d_j>=0&&head.second+d_j<keyboard[0].size()&& keyboard[head.first+d_i][head.second+d_j] != '.'){
        const string path_name = create_name(from,keyboard[head.first+d_i][head.second+d_j]);
        string temp = memo[current_name];
        if(d_j==1)
            temp.push_back('>');
        else if(d_j==-1)
            temp.push_back('<');
        else if(d_i==1)
            temp.push_back('v');
        else if(d_i==-1)
            temp.push_back('^');
        if(memo.find(path_name) == memo.end()){
            memo[path_name] = (temp);
            queue.push_back({head.first+d_i,head.second+d_j});
        }
    }
}
void find(const char& from,const char& target, map<string,string>& memo, const vector<vector<char>>& keyboard){
    // empiric path from AoC reddit 
    int i=0,j=0,i_from = 0,j_from = 0,i_target = 0, j_target=0;
    for(const vector<char>& line:keyboard){
        j = 0;
        for(const char& c:line){
            if(keyboard[i][j]==from) i_from=i,j_from=j;
            if(keyboard[i][j]==target) i_target=i,j_target=j;
            ++j;
        }
        ++i;
    }
    int i_missing = 3,j_missing = 0;
    string ret = "";
    while(keyboard[i_from][j_from]!=target){
        if(j_target<j_from){
            if(i_from==i_missing&&j_target==j_missing){
                ret += string(i_from-i_target,'^');
                i_from = i_target;
            } else{
                ret += '<';
                --j_from;
            }
        } else if(i_target<i_from){
            ret += '^';
            --i_from;
        } else if(i_target>i_from){
            if(j_from==j_missing&&i_target==i_missing){
                ret += string(j_target-j_from,'>');
                j_from = j_target;
            } else{
                ret += 'v';
                ++i_from;
            }
        } else if(j_target>j_from){
            ret += '>';
            ++j_from;
        }
    }
    memo[create_name(from,target)] = ret;
}

string convert(const string& code,map<string,string>& memo,const vector<vector<char>>& keyboard){
    char current = 'A';
    string ret;
    for(const char& c:code){
        string key(create_name(current,c));
        if(memo.find(key)==memo.end()){
            find(current,c, memo, keyboard);
        }
        current = c;
        ret += memo[key]+'A';
    }
    return ret;
}

struct Leaf{
    Leaf(const string k, const int d):key(k),depth(d){}
    string key;
    int depth;
    string hash(){
        return std::to_string(depth)+key; 
    }
};
long long recur(Leaf leaf, map<string,long long>& memo,map<string,string>& dir_memo){
    string hash = leaf.hash();
    if(leaf.depth == 0){
        memo[hash] = 1;
        return memo[hash];
    }
    if(memo.find(hash)!=memo.end()){
        return memo[hash];
    }
    string temp = "A" + dir_memo[leaf.key] + "A";
    long long ret = 0;
    for(int i = 0; i<temp.size()-1;++i){
        ret += recur(Leaf(temp.substr(i,2),leaf.depth-1),memo,dir_memo);
    }
    memo[hash] = ret;
    return ret;    
}
int main(){
    std::ifstream file("input");
    string line;
    vector<vector<char>> num_keyboard = {{'7','8','9'},{'4','5','6'},{'1','2','3'},{'.','0','A'}};
    vector<vector<char>> dir_keyboard = {{'.','^','A'},{'<','v','>'}};
    map<string,string> num_memo;
    map<string,string> dir_memo;

    // empiric path from AoC reddit 
    dir_memo = {{"A^","<"},{"A>","v"},{"Av","<v"},{"A<","v<<"},
                {"^A",">"},{"^>","v>"},{"^<","v<"},{"^v","v"},
                {"vA","^>"},{"v>",">"},{"v<","<"},{"v^","^"},
                {">A","^"},{">^","<^"},{">v","<"},{"><","<<"},
                {"<A",">>^"},{"<^",">^"},{"<v",">"},{"<>",">>"}};

    int ret_1=0;
    long long ret_2=0;long long n = 0;
    string num_to_dir_str,part_1;
    map<string,long long> memo;
    while(getline(file,line)){
        num_to_dir_str = convert(line,num_memo,num_keyboard);
        part_1 = num_to_dir_str;
        for(int i=0;i<2;++i){
            part_1 = convert(part_1,dir_memo,dir_keyboard);
        }
        string transition = "A" + num_to_dir_str;
        n = 0;
        for(int j = 0; j<transition.size()-1;++j){
            n += recur(Leaf(transition.substr(j,2),25),memo,dir_memo);
        }
        int number =  stoi(line.substr(0,3));
        ret_1 += part_1.size() * number;
        ret_2 += n * number;
    }
    cout << "Part 1: " << ret_1 << endl;
    cout << "Part 2: " << ret_2 << endl;
    return 0;
}