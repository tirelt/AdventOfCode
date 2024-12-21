#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <regex>
#include <stdexcept>
#include <cmath>

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::regex;
using std::smatch;
using std::stringstream;

long long* combo(long long* v, long long* A, long long* B,long long* C){
    switch (*v)
    {
    case 4:
        return A;
        break;
    case 5:
        return B;
        break;
    case 6:
        return C;
        break;    
    default:
        return v;
        break;
    }
    return nullptr;
}

void run(vector<long long>& instructions ,long long* A_ptr,long long* B_ptr,long long* C_ptr, bool stop_at_first, int& first_value){
    int i=0;
    while(i<instructions.size()-1){
        switch (instructions[i])
        {
        case 0:
            *A_ptr = *A_ptr/pow(2,*combo(&instructions[i+1],A_ptr,B_ptr,C_ptr));
            break;
        case 1:
            *B_ptr = *B_ptr ^ instructions[i+1];
            break;
        case 2:
            *B_ptr = *combo(&instructions[i+1],A_ptr,B_ptr,C_ptr)%8;
            break;
        case 3:
            if(*A_ptr){
                i = instructions[i+1];
                i-=2;
            }
            break;
        case 4:
            *B_ptr = *B_ptr ^ *C_ptr;
            break;
        case 5:
            first_value = *combo(&instructions[i+1],A_ptr,B_ptr,C_ptr)%8;
            cout << first_value << ',';
            if(stop_at_first)
                return;
            break;
        case 6:
            *B_ptr = *A_ptr/pow(2,*combo(&instructions[i+1],A_ptr,B_ptr,C_ptr));
            break;
        case 7:
            *C_ptr = *A_ptr/pow(2,*combo(&instructions[i+1],A_ptr,B_ptr,C_ptr));
            break;
        default:
        throw std::runtime_error("whoopsie");
            break;
        }
        i += 2;
    }
}
int main(){
    std::ifstream file("input");
    stringstream buffer; 
    buffer << file.rdbuf();
    string input = buffer.str();
    string pattern("Register A: (\\d+)\\nRegister B: (\\d+)\\nRegister C: (\\d+)\\n\\nProgram: (\\d+(,\\d+)*)");
    regex r(pattern);
    smatch results;
    regex_search(input, results, r);
    long long A = stoi(results.str(1));
    long long B = stoi(results.str(2));
    long long C = stoi(results.str(3));
    file.close();
    vector<long long> instructions;
    stringstream instructions_str(results.str(4));
    string temp;
    while(getline(instructions_str,temp,',')){
        instructions.push_back(stoi(temp));
    }
    int first_value;

    cout << "Part 1: ";
    run(instructions ,&A,&B,&C,false,first_value);
    cout << endl;
    cout<< "Part 1: " << 0 << endl;   
    return 0;
}