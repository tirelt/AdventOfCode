#include <fstream>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

int process_line(const string& line){
    decltype(line.size()) i = 0,j=line.size()-1;
    while(!isdigit(line[i])){
        ++i;
    }
    while(!isdigit(line[j])){
        --j;
    }
    return (line[i] -'0')*10+(line[j] -'0');
}

int process_line_aux(const string& line, string::size_type i, bool increase){
    while(0<=i && i<line.size()){
        if(isdigit(line[i])){
            return line[i] -'0';
        } 
        if(i + 2 < line.size()){
            const string& substring = line.substr(i, 3);
            if(substring=="one"){
                return 1;
            } else if(substring=="two"){
                return 2;
            } else if(substring=="six"){
                return 6;
            }
        }
        if(i + 3 < line.size()){
            const string& substring = line.substr(i, 4);
            if(substring=="four"){
                return 4;
            } else if(substring=="five"){
                return 5;
            } else if(substring=="nine"){
                return 9;
            }
        } 
        if(i + 4 < line.size()){
            const string& substring = line.substr(i, 5);
            if(substring=="three"){
                return 3;
            } else if(substring=="seven"){
                return 7;
            } else if(substring=="eight"){
                return 8;
            }
        } 
        increase?++i:--i;
    }    
    throw std::runtime_error("No number");
}

int process_line_smart(const string& line){
    int first_digit = process_line_aux(line,0,true);
    int second_digit = process_line_aux(line,line.size()-1,false);
    return first_digit*10+second_digit;
}

int main(){
    std::ifstream file("input");
    string line;
    int sum = 0;
    while(file>>line){
        sum += process_line_smart(line);
    }
    file.close();
    cout<< "The calibreation value is: "<< sum << endl; 
    return 0;
}