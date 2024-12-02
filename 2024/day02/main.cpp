#include <fstream>
#include <iostream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::sort;
using std::istringstream;

bool is_safe(vector<int>& report){
    bool increasing = report[0] <= report[1];
    for(unsigned i=0;i<report.size()-1;++i){
        unsigned diff = abs(report[i]-report[i+1]);
        if( diff > 0 && diff < 4 ){
            if(increasing){
                if(report[i+1]<report[i])
                    return false;
            } else{
                if(report[i+1]>report[i])
                    return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

int main(){
    std::ifstream file("input");
    string line;
    vector<int> report;
    string entry;
    unsigned res = 0;
    while(getline(file,line)){
        report.clear();
        istringstream entries(line,' ');
        while (entries >> entry){
            report.push_back(stoi(entry));
        }
        if(is_safe(report))
            res += 1;
    }
    file.close();
    cout<< "The number of safe report is: " << res << endl;  
    return 0;
}