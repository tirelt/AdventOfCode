#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::map;
using std::set;
using std::pair;

using network_type = map<string,map<char,string>>;

void process_line(network_type& network, vector<string>& positions,string& line){
    map<char,string> node;
    node['L'] = line.substr(7,3);
    node['R'] = line.substr(12,3);
    network[line.substr(0,3)] = node;
    if (line[2]=='A'){
        positions.push_back(line.substr(0,3));
    }
}

void get_direction(vector<char>& directions,string& line){
    for(const char& c : line){
        directions.push_back(c);
    }
}


int main(){
    std::ifstream file("input");
    string line;
    vector<char> directions;
    vector<string> positions;
    network_type network;
    //First line -> Directions
    getline(file,line);
    get_direction(directions,line);

    // Second Line -> empty
    getline(file,line);

    // Rest Network
    getline(file,line);
    process_line(network,positions,line);
    string current_position = "AAA";//line.substr(0,3);
    while(getline(file,line)){
        process_line(network,positions,line);
    }
    file.close();

    //Crawl Network part 1
    unsigned count = 0;
    while( current_position != "ZZZ" && count < 1000000){
        current_position = network[current_position][directions[count++%directions.size()]];
    }
    cout<< "Part 1: The number of steps required to reach ZZZ is " << count << endl;

    //Crawl network par 2
    vector<unsigned> counts;
    unsigned count_cycles = 0;
    set<pair<string,unsigned>> states;
    unsigned direction_number;
    vector<vector<long long>> positions_z;
    vector<long long> positions_z_temp;
    vector<vector<pair<string,unsigned>>> current_positions;
    vector<pair<string,unsigned>> current_positions_temp;
    for(const string& p: positions){
        string current_position = p;
        count_cycles = 0;
        states.clear();
        positions_z_temp.clear();
        current_positions_temp.clear();
        auto ret = states.insert({current_position,0});
        current_positions_temp.push_back({current_position,0});
        while( ret.second && count_cycles < 25000){ //ret.second 
            direction_number = count_cycles++%directions.size();
            current_position = network[current_position][directions[direction_number]];
            ret = states.insert({current_position,direction_number+1});
            if(current_position[2] == 'Z'){
                positions_z_temp.push_back(count_cycles);
            }
            current_positions_temp.push_back({current_position,direction_number+1});
        }
        current_positions.push_back(current_positions_temp);
        positions_z.push_back(positions_z_temp);
        counts.push_back(count_cycles);
    }
    /*
    We notice for each starting position, the process cycles shortly after discovering the first
    ..Z position. Furthermore the ..Z position is found after going through the whole directions chain
    a round number of times and this number is prime. So the shorted way for all started positions
    to end on ..Z position is to go through the directions chain a number of times equals to the 
    products of the number_of_chains needed for each.
    */
    long long ret = 1;
    unsigned temp;
    for(unsigned i = 0;i<positions.size();++i){
        temp = positions_z[i][0] / directions.size();
        ret *= temp;
    }
    cout<< "Part 2: The number of steps required to reach ..Z is " << ret * directions.size() << endl; 
    return 0;
}