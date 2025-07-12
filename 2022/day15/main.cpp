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

bool possible(const pair<pair<int, int>,int>& a, const pair<pair<int, int>,int>& b) {
    int diag_dist = abs((a.first.first - a.first.second) - (b.first.first - b.first.second));
    int comp = a.second + b.second + 2;
    return diag_dist == comp;
}

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

    /*
    Intuition for part 2:
    We need to find a point that needs to be at 1 unit away from 4 sensors and not too close to any other sensor.
    So for each 4 sensors we can set up a system of equations that describes the point and we solve it,
    Then we check if the point is valid, i.e. not too close to any other sensor.

    The maximum of different combinations of 4 sensors is 32 choose 4 = 35,960. O(n^4) where n is the number of sensors
    For each combination the sign of the abs value can take 9 differnet values. 
    So we solve at most 35,960 * 9 = 323,640 systems of equations.

    Solving a system is O(1). 

    Checking if a solution is valid is O(n).

    So the overall complexity is O(n^5).
    */
    int checks = 0;
    for(int i = 0; i < sensors.size(); ++i){
        for(int j = i+1; j < sensors.size(); ++j){
            for(int k = j+1; k < sensors.size(); ++k){
                for(int l = k+1; l < sensors.size(); ++l){
                    int a, b, c, d, e, f, g, h;
                    map<int,int*> x_coords = {{sensors[i].first.first,&a}, {sensors[j].first.first,&b}, {sensors[k].first.first,&c}, {sensors[l].first.first ,&d}};
                    map<int,int*> y_coords = {{sensors[i].first.second,&e}, {sensors[j].first.second,&f}, {sensors[k].first.second,&g}, {sensors[l].first.second ,&h}};
                    vector<vector<int>> possibilities = {{1,-1,-1,-1},{1,1,-1,-1},{1,1,1,-1}};
                    for(int n = 0; n < possibilities.size(); ++n){
                        int counter = 0;
                        for( auto& x : x_coords)
                            *x.second = possibilities[n][counter++];
                        for(int m = 0; m < possibilities.size(); ++m){
                            counter = 0;
                            for( auto& y : y_coords)
                                *y.second = possibilities[m][counter++];
                            int a_ = a*a + b*b + c*c + d*d;
                            int b_ = a*e + b*f + c*g + d*h;
                            int d_ = e*e + f*f + g*g + h*h;
                            double det = a_*d_ - b_*b_;
                            int r = sensors[i].second + 1 + a * sensors[i].first.first + e * sensors[i].first.second;
                            int s = sensors[j].second + 1 + b * sensors[j].first.first + f * sensors[j].first.second;
                            int t = sensors[k].second + 1 + c * sensors[k].first.first + g * sensors[k].first.second;
                            int u = sensors[l].second + 1 + d * sensors[l].first.first + h * sensors[l].first.second; 
                            int y  = a * r + b * s + c * t + d * u;
                            int z = e * r + f * s + g * t + h * u;
                            checks++;
                            if (abs(det)>1e-6 ){
                                double v = (y * d_ - z * b_) / det;
                                double w = (- y * b_ + z * a_) / det; 
                                double test_1 = a*(v-sensors[i].first.first) + e*(w-sensors[i].first.second) - (sensors[i].second + 1);
                                double test_2 = b*(v-sensors[j].first.first) + f*(w-sensors[j].first.second) - (sensors[j].second + 1);
                                double test_3 = c*(v-sensors[k].first.first) + g*(w-sensors[k].first.second) - (sensors[k].second + 1);
                                double test_4 = d*(v-sensors[l].first.first) + h*(w-sensors[l].first.second) - (sensors[l].second + 1);
                                if (abs(test_1) < 1e-6 && abs(test_2) < 1e-6 && abs(test_3) < 1e-6 && abs(test_4) < 1e-6 && v >= 0 && w >= 0 && v <= 4e6 && w <= 4e6) {
                                    bool too_close = false;
                                    for(int p = 0; p < sensors.size(); ++p){
                                        if (p != i && p != j && p != k && p != l){
                                            if (abs(sensors[p].first.first - v) + abs(sensors[p].first.second - w) <= sensors[p].second) {
                                                too_close = true;
                                                break;
                                            }
                                        }
                                    }
                                    if(!too_close) {
                                        long long res_2 = v * 4e6 + w;
                                        cout << "Part 2: " << res_2  << endl;
                                        return 0;
                                    }
                                }
                            }  
                        }
                    } 
                }
            }
        }
    }
    return 0;
}