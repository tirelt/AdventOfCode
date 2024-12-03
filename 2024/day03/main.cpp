#include <fstream>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

long long process(string& memory,bool use_dos){
    string model = "mul(?,?)";
    long long res = 0;
    unsigned i=0,j=0,old_i=0;
    string digits;
    bool enabled=true;
    while(i<memory.size()-7){
        if(use_dos){
            if(memory[i]=='d'){
                if(memory.substr(i,4)=="do()"){
                    enabled = true;
                    i+=4;
                }
                if(memory.substr(i,7)=="don't()"){
                    enabled = false;
                    i+=7;
                }
            }
        }
        if(memory[i]=='m' && enabled){
            old_i = i;
            j = 0;
            long long temp = 1;
            while(j<model.size()){
                if(model[j]!='?'){
                    if(model[j]==memory[i]){
                        ++i;
                        ++j;
                    } else{
                        break;
                    }
                } else if(digits.size() < 3 && isdigit(memory[i])){
                    digits.push_back(memory[i]);
                    ++i;
                } else if(digits.size()){
                    ++j;
                    temp *= stoi(digits);
                    digits.clear();
                }
                else {
                    break;
                }
            }
            if(j==model.size()){
                res += temp;
            }
            i = old_i + 1;
        } else{
            ++i;
        }
    }
    return res;
}
int main(){
    std::ifstream file("input");
    string line;
    string memory;
    while(getline(file,line)){
        for(char const &c: line){
            memory.push_back(c);
        }
    }
    file.close();
    cout<< "Part 1: " << process(memory,false) << endl;  
    cout<< "Part 2: " << process(memory,true) << endl;  
    return 0;
}