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
using std::min;
using std::max;
using std::make_shared;
using std::shared_ptr;
using std::pair;

void get_numbers_from_line(string& line,vector<long long>& output){
    unsigned long long i=0,transformation;
    while(i<line.size()){
        const char& c = line[i];
        if(isdigit(c)){
            transformation = i + 1;
            while(transformation<line.size() && isdigit(line[transformation])){
                ++transformation;
            }       
            unsigned long long n = 0;
            for( unsigned long long l = i;l<transformation;++l){
                n += (line[l]-'0')*pow(10,transformation-l-1);
            }
            output.push_back(n);
            i = transformation;
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

    // PARSE FILE
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
            //get_numbers_from_line(line,line_numbers);

        }else{
            continue;
        }
    }
    if(bulding_map){
        map_list->push_back(current_map);
    }
    
    // PART 1
    vector<unsigned long long> mapped_values;
    string step_name;
    for(auto seed:seeds){
        auto mapped_value = seed;
        for(auto i = map_list->begin();i!=map_list->end();++i){
            step_name = mapping_keyword[i-map_list->begin()];
            for(auto transformation = i->begin();transformation!=i->end();++transformation){
                if(mapped_value>=(*transformation)[1] && mapped_value<=(*transformation)[1]+(*transformation)[2]-1){
                    mapped_value += (*transformation)[0]-(*transformation)[1];
                    break;
                }
            }
        }
        mapped_values.push_back(mapped_value);
    }
    auto min_ite = std::min_element(mapped_values.begin(),mapped_values.end());
     
    // PART 2 
    vector<pair<long long,long long>> seed_ranges;
    vector<vector<pair<long long,long long>>> all_value_ranges;
    vector<pair<long long,long long>> value_ranges;
    vector<pair<long long,long long>> value_ranges_to_process;
    vector<pair<long long,long long>> new_value_ranges;
    pair<long long,long long> new_range;
    long long lower_bound,upper_bound,shift;
    for(unsigned i=0;i<seeds.size()/2;++i){
        seed_ranges.push_back({seeds[i*2],seeds[i*2]+seeds[i*2+1]-1});
    }
    all_value_ranges.push_back(seed_ranges);
    value_ranges = seed_ranges;
    for(auto step = map_list->begin();step!=map_list->end();++step){
        step_name = mapping_keyword[step-map_list->begin()]; 
        for(auto transformation = step->begin();transformation!=step->end();++transformation){
            value_ranges_to_process.clear();
            for(auto& value_range:value_ranges){  
                lower_bound=(*transformation)[1],upper_bound=(*transformation)[1]+(*transformation)[2]-1,shift=(*transformation)[0]-(*transformation)[1];
                if(value_range.first>upper_bound || value_range.second<lower_bound){
                    value_ranges_to_process.push_back(value_range);
                } else if(value_range.first<lower_bound && value_range.second<=upper_bound){
                    value_ranges_to_process.push_back({value_range.first,lower_bound-1});
                    new_value_ranges.push_back({lower_bound+shift,value_range.second+shift});
                } else if(value_range.first>=lower_bound && value_range.second>upper_bound) {
                    new_value_ranges.push_back({value_range.first+shift,upper_bound+shift});
                    value_ranges_to_process.push_back({upper_bound+1,value_range.second});
                } else if(value_range.first>=lower_bound && value_range.second<=upper_bound){
                    new_value_ranges.push_back({value_range.first+shift,value_range.second+shift});
                } else if(value_range.first<lower_bound && value_range.second>upper_bound){
                    value_ranges_to_process.push_back({value_range.first,lower_bound-1});
                    new_value_ranges.push_back({lower_bound+shift,upper_bound+shift});
                    value_ranges_to_process.push_back({upper_bound+1,value_range.second});
                } else{
                    throw std::runtime_error("Case forgotten");
                }
            }
            value_ranges=value_ranges_to_process;
        }
        value_ranges = new_value_ranges;
        value_ranges.insert(value_ranges.end(),value_ranges_to_process.begin(),value_ranges_to_process.end());
        all_value_ranges.push_back(value_ranges);
        new_value_ranges.clear();
    }
    auto part_2_min = all_value_ranges[all_value_ranges.size()-1][0].first;
    for(auto range: all_value_ranges[all_value_ranges.size()-1]){
        part_2_min = min(part_2_min,range.first);
    }
    
    file.close();
    cout<< "Part 1 - The lowest location number that corresponds to any of the initial seed numbers is: "<< *min_ite << endl; 
    cout<< "Part 2 - The lowest location number that corresponds to any of the initial seed numbers is: "<< part_2_min << endl; 
    return 0;
}