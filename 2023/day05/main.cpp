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


void get_numbers_from_line(string& line,vector<long long>& output){
    unsigned long long i=0,j;
    while(i<line.size()){
        const char& c = line[i];
        if(isdigit(c)){
            j = i + 1;
            while(j<line.size() && isdigit(line[j])){
                ++j;
            }       
            unsigned long long n = 0;
            for( unsigned long long l = i;l<j;++l){
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
    std::ifstream file("input");
    string line;
    vector<long long> seeds;
    vector<long long> line_numbers;
    shared_ptr<vector<vector<vector<long long>>>> map_list = make_shared<vector<vector<vector<long long>>>>();
    vector<vector<long long>> current_map;
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
            line_numbers.clear();
            get_numbers_from_line(line,line_numbers);
            current_map.push_back(line_numbers);
        } else if(line.substr(0,6)=="seeds:"){
            get_numbers_from_line(line,seeds);
        }else{
            continue;
        }
    }
    if(bulding_map){
        map_list->push_back(current_map);
    }
    vector<unsigned long long> mapped_values;
    string step;
    for(auto seed:seeds){
        auto mapped_value = seed;
        for(auto i = map_list->begin();i!=map_list->end();++i){
            step = mapping_keyword[i-map_list->begin()];
            for(auto j = i->begin();j!=i->end();++j){
                if(mapped_value>=(*j)[1] && mapped_value<=(*j)[1]+(*j)[2]-1){
                    mapped_value += (*j)[0]-(*j)[1];
                    break;
                }
            }
        }
        mapped_values.push_back(mapped_value);
    }
    auto min_ite = std::min_element(mapped_values.begin(),mapped_values.end());
    file.close();
    cout<< "The sum ot the points of the cards is: "<< *min_ite << endl; 
    return 0;
}