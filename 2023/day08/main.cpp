#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::map;

using network_type = map<string,map<char,string>>;

void process_line(network_type& network, string& line){
    map<char,string> node;
    node['L'] = line.substr(7,3);
    node['R'] = line.substr(12,3);
    network[line.substr(0,3)] = node;
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
    network_type network;
    //First line -> Directions
    getline(file,line);
    get_direction(directions,line);

    // Second Line -> empty
    getline(file,line);

    // Rest Network
    getline(file,line);
    process_line(network,line);
    string current_position = "AAA";//line.substr(0,3);
    while(getline(file,line)){
        process_line(network,line);
    }
    file.close();

    //Crawl Network
    unsigned count = 0;
    while( current_position != "ZZZ" && count < 1000000){
        current_position = network[current_position][directions[count++%directions.size()]];
    }
    cout<< "The number of steps required to reach ZZZ is " << count << endl; 
    return 0;
}