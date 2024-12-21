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
    bool first =true;
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
            if(first)
                first_value = *combo(&instructions[i+1],A_ptr,B_ptr,C_ptr)%8;
                first=false;
            if(stop_at_first)
                return;
            cout << *combo(&instructions[i+1],A_ptr,B_ptr,C_ptr)%8 << ',';
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

long long find(vector<long long>& instructions,int pos,long long starting_point,int& rec_calls){
    /*
    Dark magic relying on the recurrence, output(A)[n] =  output(A*8)[n+1]
    Why does this work ? we don't know and are too lazy to figure out.
    This algo doens't assume any reccurence in the function \x -> output(x)[0]. So at each steps it check the next 8 numbers, if it 
    not within the first 8 we need to change the previously found number
    */
    ++rec_calls;
    if(pos == - 1){
        return starting_point/8;
    }
    long long A,B,C,ret = -1;
    int first_value;
    bool stop_at_first =true;
    for(long long i=starting_point;i<starting_point+8;++i){
        A = i;
        B = 0;
        C = 0;
        run(instructions ,&A,&B,&C,stop_at_first,first_value);
        if(!stop_at_first) cout<<endl;
        if(first_value==instructions[pos]){
            ret = find(instructions,pos-1,i*8,rec_calls);
            if(ret!=-1){
                return ret;
            }
        }
    }
    return ret;
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
    
    long long ret;
    int rec_calls=0;
    ret = find(instructions,instructions.size()-1,0,rec_calls);
    cout<< "Part 2: " << ret << endl;
    return 0;
}