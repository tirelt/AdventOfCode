#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <map>
#include <set>
#include <memory>
#include <utility>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::find;
using std::map;
using std::make_shared;
using std::shared_ptr;


void get_numbers_from_line(string& line,vector<unsigned>& output){
    unsigned int i=0,j;
    while(i<line.size()){
        const char& c = line[i];
        if(isdigit(c)){
            j = i + 1;
            while(j<line.size() && isdigit(line[j])){
                ++j;
            }       
            unsigned int n = 0;
            for( unsigned int l = i;l<j;++l){
                n += (line[l]-'0')*pow(10,j-l-1);
            }
            output.push_back(n);
            i = j;
        } else {
            ++i;
        }
    }
}

int main(){
    std::ifstream file("test_input");
    string line;
    vector<unsigned> seeds;
    vector<unsigned> line_numbers;
    shared_ptr<vector<map<unsigned,unsigned>>> map_list = make_shared<vector<map<unsigned,unsigned>>>();
    map<unsigned,unsigned> current_map;
    vector<string> mapping_keyword({"seed-to-soil map:","soil-to-fertilizer map:","fertilizer-to-water map:","water-to-light map:","light-to-temperature map:","temperature-to-humidity map:","humidity-to-location map:"});
    bool bulding_map = false;
    while(getline(file,line)){
        if(line.empty()){
            if(bulding_map){
                map_list->push_back(current_map);
            }
            bulding_map = false;
            current_map.clear();
        } else if(find(mapping_keyword.begin(),mapping_keyword.end(),line)!=mapping_keyword.end()){
            bulding_map = true;
        } else if(bulding_map){
            get_numbers_from_line(line,line_numbers);
            for(unsigned i=0;i<line_numbers[2];++i){
                current_map.insert({line_numbers[1]+i,line_numbers[0]+i});
            }
        } else if(line.substr(0,6)=="seeds:"){
            get_numbers_from_line(line,seeds);
        }else{
            continue;
        }
    }
    file.close();
    cout<< "The sum ot the points of the cards is: "<< 0 << endl; 
    return 0;
}