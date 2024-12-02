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

bool is_safe(vector<int>& report,unsigned tolerance){
    unsigned i=0,j=1;
    //check first if we need to exclude first value
    vector<int> diff{report[1]-report[0],report[2]-report[1],report[3]-report[2]};
    bool exclude_first = false;
    if(tolerance>0 && diff[1]>0 && diff[1]<4 && diff[2]>0 && diff[2]<4){
        exclude_first = !(diff[0]>0 && diff[0]<4);
    } else if(tolerance>0 && diff[1]<0 && diff[1]>-4 && diff[2]<0 && diff[2]>-4){
        exclude_first = !(diff[0]<0 && diff[0]>-4);
    } 
    if(exclude_first){
        --tolerance;
        i=j++;
    }
    int sign_diff=0;
    for(int const& d: diff){
        if(d>0)
            ++sign_diff;
        else
            --sign_diff;
    }
    bool increasing = sign_diff>=0;
    while(j<report.size()){
        bool fail = false;
        unsigned diff = abs(report[i]-report[j]);
        if( diff > 0 && diff < 4 ){
            if(increasing){
                if(report[j]<report[i]){
                    if(!tolerance){
                        return false;
                    } else{
                        fail = true;                        
                    }
                }
            } else{
                if(report[j]>report[i]){
                    if(!tolerance){
                        return false;
                    } else{
                        fail = true;                        
                    }
                }
            }
        } else {
            if(!tolerance){
                return false;
            } else{
                fail = true;                        
            }
        }
        if(fail){
            ++j;
            --tolerance;
        } else {
            i=j++;
        }
    }
    return true;
}

int main(){
    std::ifstream file("test_input");
    string line;
    vector<int> report;
    string entry;
    unsigned res = 0;
    unsigned res_ = 0;
    unsigned counter = 1;
    while(getline(file,line)){
        report.clear();
        istringstream entries(line,' ');
        while (entries >> entry){
            report.push_back(stoi(entry));
        }
        bool safe = is_safe(report,0);
        unsigned i = 0;
        while(i<report.size() && !safe){
            vector<int> myVector = report;
            myVector.erase(myVector.begin() + i);
            safe = safe || is_safe(myVector,0);
            ++i;
        }
        if(is_safe(report,1)){ //tolerance=0 for part 1, tolerance=1 for part 2 
            ++res;
        }
        if(safe){
            ++res_;
        }
        if(is_safe(report,1) != safe){
            cout<< counter << endl;  
        }
        ++counter;
        //cout<< counter++ << "  " << is_safe(report,0) << " " << is_safe(report,1) << " " << safe << endl;  
    }
    file.close();
    cout<< "The number of safe report is: " << res << " " << res_  << endl;  
    return 0;
}