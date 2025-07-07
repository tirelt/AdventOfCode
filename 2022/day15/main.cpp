#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <set>
#include <map>


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stoi;
using std::abs;
using std::pair;
using std::sort;
using std::set;
using std::map;

int main(){
    std::ifstream file("input");
    string line;
    std::regex pattern(R"(Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))");
    int y_target = 2000000; 
    vector<pair<int, int>> intervals; 
    set<pair<int, int>> beacons;
    vector<pair<pair<int, int>,int>> sensors;
    while(getline(file,line)){
        std::smatch matches;
        if (std::regex_match(line, matches, pattern)) {
            int sensor_x = stoi(matches[1]);
            int sensor_y = stoi(matches[2]);
            int beacon_x = stoi(matches[3]);
            int beacon_y = stoi(matches[4]);
            if (beacon_y == y_target)
                beacons.insert({beacon_x, beacon_y});
            int distance_to_beacon = abs(sensor_x - beacon_x) + abs(sensor_y - beacon_y);
            sensors.push_back({{sensor_x, sensor_y},distance_to_beacon});
            int distance_to_target = abs(sensor_y - y_target); 
            if ( distance_to_target <= distance_to_beacon) {
                int x_distance = distance_to_beacon - distance_to_target; 
                int left_bound = sensor_x - x_distance; 
                int right_bound = sensor_x + x_distance+1;  // +1 to make it left exclusive
                intervals.push_back({left_bound, right_bound});
            }
        }
    }
    file.close();

    vector<pair<int, int>> smart_intervals;
    for(const auto& interval : intervals) {
        smart_intervals.emplace_back(interval.first,1);
        smart_intervals.emplace_back(interval.second,-1);
    }
    for(const auto& beacon : beacons) {
        smart_intervals.emplace_back(beacon.first,0);
    }
    sort(smart_intervals.begin(), smart_intervals.end(),[](const pair<int, int>& a,const pair<int, int>& b){
        return a.first < b.first;});
    int current_count = 0;
    int i = 0;
    int prev_x;
    int res_1 = 0;
    int beacon_count = 0;
    while(i < smart_intervals.size()){
        int current_x = smart_intervals[i].first;
        int count_at_x = 0;
        while(i < smart_intervals.size() && smart_intervals[i].first == current_x) {
            count_at_x += smart_intervals[i].second;
            if (smart_intervals[i].second == 0)
                beacon_count++;
            ++i;
        }
        if (current_count == 0 && count_at_x > 0) {
            prev_x = current_x;
            current_count += count_at_x;
        } else if (current_count > 0 && (current_count+count_at_x)<=0) {
            current_count += count_at_x;
            res_1 += current_x - prev_x - beacon_count; 
            beacon_count = 0;
        } else 
            current_count += count_at_x;
    }
    cout<< "Part 1: " << res_1 << endl; 

    int res_2 = 0;
    for(int i = 0; i < sensors.size(); ++i){
        for(int j = i+1; j < sensors.size(); ++j){
            for(int k = j+1; k < sensors.size(); ++k){
                for(int l = k+1; l < sensors.size(); ++l){
                    res_2++;
                }
            }
        }
    }
    cout<< "Part 2: " << 0 << endl; 

    return 0;
}