#include <fstream>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;


int main(){
    std::ifstream file("input");
    string line;
    int current = 0;
    int top[]{0, 0, 0};
    while(getline(file,line)){
        if(line.size()){
            current += stoi(line);
        } else {
            int min_index = 0;
            for(int i = 1; i < sizeof(top)/sizeof(int); ++i) {
                if(top[i] < top[min_index])
                    min_index = i;
            }
            if(current > top[min_index])
                top[min_index] = current;
            current = 0;
        }
    }
    file.close();
    int max = 0, sum = 0;
    for(int i = 0; i < 3; ++i){
        sum += top[i];
        if(top[i] > max)
            max = top[i];
    }   
    cout<< "Part 1: "<< max << endl; 
    cout<< "Part 2: "<< sum << endl; 
    return 0;
}