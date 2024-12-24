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
    ret.push_back('-');
    ret.push_back(b);
    return ret;
}
int score(const string& s){
    char ref = s.front();
    int ret = 0;
    for(const char& c:s){
        ++ret;
        if(c!=ref){
            ret+=100;
            ref = c;
        }
    }
    return ret;
}

void find_aux(const pair<int,int>& head, int d_i, int d_j,list<pair<int,int>>& queue, const char& from, map<string,set<string>>& memo, const vector<vector<char>>& keyboard){
    string current_name = create_name(from,keyboard[head.first][head.second]);
    if(head.first+d_i>=0&&head.first+d_i<keyboard.size()&&head.second+d_j>=0&&head.second+d_j<keyboard[0].size()&& keyboard[head.first+d_i][head.second+d_j] != '.'){
        const string path_name = create_name(from,keyboard[head.first+d_i][head.second+d_j]);
        for(string temp:memo[current_name]){
            if(d_j==1)
                temp.push_back('>');
            else if(d_j==-1)
                temp.push_back('<');
            else if(d_i==1)
                temp.push_back('v');
            else if(d_i==-1)
                temp.push_back('^');
            if(memo.find(path_name) != memo.end()){
                int old_score = score(*memo[path_name].begin());
                int new_score = score(temp);
                if(old_score>new_score){
                    memo[path_name] = {temp};
                } else if(old_score==new_score){
                    memo[path_name].insert(temp);
                }
            } else{
                memo[path_name].insert(temp);
                queue.push_back({head.first+d_i,head.second+d_j});
            }
        }
    }

}
void find(const char& from, map<string,set<string>>& memo, const vector<vector<char>>& keyboard){
    int i = 0,j = 0;
    for(const vector<char>& line:keyboard){
        j = 0;
        for(const char& c:line){
            if(keyboard[i][j]==from) break;
            ++j;
        }
        if(keyboard[i][j]==from) break;
        ++i;
    }
    list<pair<int,int>> queue{{i,j}};
    memo[create_name(from,from)] = {""};
    while(queue.size()){
        const pair<int,int> head = queue.front();
        queue.pop_front();
        find_aux(head, 0, 1, queue, from, memo, keyboard);
        find_aux(head, 0, -1, queue, from, memo, keyboard);
        find_aux(head, 1, 0, queue, from, memo, keyboard);
        find_aux(head, -1, 0, queue, from, memo, keyboard);
    }
}

int min_size(list<string>& l){
    int ret = 999;
    for(const auto& el:l){
        ret = std::min(int(el.size()),ret);
    }
    return ret;
}

list<string> convert(const list<string>& codes,map<string,set<string>>& memo,const vector<vector<char>>& keyboard){
    char current = 'A';
    list<string> ret;
    list<string> current_ret;
    list<string> new_ret;
    for(const string& code:codes){
        current_ret = {""};
        for(const char& c:code){
            new_ret.clear();
            string key(create_name(current,c));
            if(memo.find(key)==memo.end()){
                find(current, memo, keyboard);
            }
            current = c;
            for(const auto& str:memo[key]){
                for(auto& temp:current_ret){
                    new_ret.push_back(temp+str+'A');
                }
            }
            current_ret = new_ret;
        }
        ret.splice(ret.end(),current_ret);
    }
    int size = min_size(ret);
    list<string> ret_final;
    for(const string& r:ret){
        if(r.size()==size)
            ret_final.push_back(r);
    }
    return ret_final;
}

int main(){
    std::ifstream file("input");
    string line;
    vector<vector<char>> num_keyboard = {{'7','8','9'},{'4','5','6'},{'1','2','3'},{'.','0','A'}};
    vector<vector<char>> dir_keyboard = {{'.','^','A'},{'<','v','>'}};
    map<string,set<string>> num_memo;
    map<string,set<string>> dir_memo;
    list<string> temp;
    int ret_1=0,a,b;
    while(getline(file,line)){
        temp = convert({line},num_memo,num_keyboard);
        for(int i=0;i<2;++i){
            temp = convert(temp,dir_memo,dir_keyboard);
        }
        a = min_size(temp);
        b =  stoi(line.substr(0,3));
        ret_1 += a * b;
    }
    cout << "Part 1: " << ret_1 << endl;
    return 0;
}