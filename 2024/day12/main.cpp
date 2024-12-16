#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::vector;
using std::pair;
using std::set;
using std::list;

unsigned hash_pos(unsigned i, unsigned j){
    return i*1000+j;
}
unsigned hash_barrier(unsigned d,unsigned i, unsigned j){
    return d*1000000+i*1000+j;
}
int main(){
    std::ifstream file("input");
    string line;
    long long ret_1 = 0;
    long long ret_2 = 0;
    map<long long,long long> stones;
    vector<vector<char>> input;
    while(getline(file,line)){
        vector<char> temp;
        for(const char& c:line){
            temp.emplace_back(c);
        }
        input.emplace_back(temp);
    }
    set<unsigned> in_plot;
    vector<list<pair<unsigned,unsigned>>> plots;
    set<unsigned> seen;
    set<unsigned> barriers;
    unsigned hash;
    unsigned area = 0,side = 0, perimeter = 0;
    char ref;
    for(unsigned i=0;i<input.size();++i){
        for(unsigned j=0;j<input[0].size();++j){
            ref = input[i][j];
            hash = hash_pos(i,j);
            list<pair<unsigned,unsigned>> plot;
            if(in_plot.find(hash)==in_plot.end()){
                list<pair<unsigned,unsigned>> queue{{i,j}};
                seen.clear();
                barriers.clear();
                area = 0,side = 0, perimeter = 0;
                while(queue.size()){
                    pair<unsigned,unsigned> coord(queue.front());
                    queue.pop_front();
                    in_plot.insert(hash_pos(coord.first,coord.second));
                    area +=1;
                    plot.emplace_back(coord.first,coord.second);
                    if(coord.first>=1&&ref==input[coord.first-1][coord.second]){
                        if(seen.find(hash_pos(coord.first-1,coord.second))==seen.end()){
                            seen.insert(hash_pos(coord.first-1,coord.second));
                            queue.emplace_back(coord.first-1,coord.second);
                        }
                    } else {
                        perimeter += 1;
                        barriers.insert(hash_barrier(0,coord.first,coord.second));
                        if((coord.second>=1?barriers.find(hash_barrier(0,coord.first,coord.second-1))==barriers.end():true)&& 
                            (coord.second<(input[0].size()-1)?barriers.find(hash_barrier(0,coord.first,coord.second+1))==barriers.end():true)){
                            side += 1;
                        }
                    }
                    if(coord.first<(input.size()-1)&&ref==input[coord.first+1][coord.second]){
                        if(seen.find(hash_pos(coord.first+1,coord.second))==seen.end()){
                            seen.insert(hash_pos(coord.first+1,coord.second));
                            queue.emplace_back(coord.first+1,coord.second);
                        }
                    } else {
                        perimeter += 1;
                        barriers.insert(hash_barrier(1,coord.first,coord.second));
                        if((coord.second>=1?barriers.find(hash_barrier(1,coord.first,coord.second-1))==barriers.end():true)&& 
                            (coord.second<(input[0].size()-1)?barriers.find(hash_barrier(1,coord.first,coord.second+1))==barriers.end():true)){
                            side += 1;
                        }
                    }
                    if(coord.second>=1&&ref==input[coord.first][coord.second-1]){
                        if(seen.find(hash_pos(coord.first,coord.second-1))==seen.end()){
                            seen.insert(hash_pos(coord.first,coord.second-1));
                            queue.emplace_back(coord.first,coord.second-1);
                        }
                    } else {
                        perimeter += 1;
                        barriers.insert(hash_barrier(2,coord.first,coord.second));
                        if((coord.first>=1?barriers.find(hash_barrier(2,coord.first-1,coord.second))==barriers.end():true)&& 
                            (coord.first<(input.size()-1)?barriers.find(hash_barrier(2,coord.first+1,coord.second))==barriers.end():true)){
                            side += 1;
                        }
                    }
                    if(coord.second<(input[0].size()-1)&&ref==input[coord.first][coord.second+1]){
                        if(seen.find(hash_pos(coord.first,coord.second+1))==seen.end()){
                            seen.insert(hash_pos(coord.first,coord.second+1));
                            queue.emplace_back(coord.first,coord.second+1);
                        }
                    } else {
                        perimeter += 1;
                        barriers.insert(hash_barrier(3,coord.first,coord.second));
                        if((coord.first>=1?barriers.find(hash_barrier(3,coord.first-1,coord.second))==barriers.end():true)&& 
                            (coord.first<(input.size()-1)?barriers.find(hash_barrier(3,coord.first+1,coord.second))==barriers.end():true)){
                            side += 1;
                        }
                    }
                    seen.insert(hash_pos(coord.first,coord.second));
                    
                }
                ret_1 += perimeter * area;
                ret_2 += side * area;
            }
        }
    }
    cout<< "Part 1: " << ret_1 << endl;   
    cout<< "Part 2: " << ret_2 << endl;  
    return 0;
}