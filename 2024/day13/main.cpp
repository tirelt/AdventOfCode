#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <regex>
#include <cmath>

using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::ifstream;
using std::stringstream;
using std::sregex_iterator;
#include <cfloat>

bool isInteger(double num){
    return std::floor(num) == num; 
}

long long get_cost(double x_a,double y_a,double x_b,double y_b,long long x,long long y,long long shift = 0, unsigned price_a = 3,unsigned price_b = 1){
    x += shift;
    y += shift;
    double d = x_a * y_b - y_a * x_b, n_a, n_b;
    if(d){
        n_a = (y_b * x - x_b * y ) / d;
        n_b = (-y_a * x + x_a * y ) / d;
        if(isInteger(n_a)&&isInteger(n_b)){
            return n_a*price_a+n_b*price_b;
        }
    }
    return 0;
}

int main(){
    std::ifstream file("input");
    std::stringstream buffer; 
    buffer << file.rdbuf();
    string input = buffer.str();
    string line;
    long long ret_1 = 0;
    long long ret_2 = 0;
    string pattern("Button A: X\\+(\\d+), Y\\+(\\d+)\nButton B: X\\+(\\d+), Y\\+(\\d+)\nPrize: X\\=(\\d+), Y\\=(\\d+)");
    regex r(pattern);
    for(sregex_iterator ite(input.begin(), input.end(), r), end_it;ite != end_it; ++ite){
        ret_1 += get_cost(stoi(ite->str(1)),stoi(ite->str(2)),stoi(ite->str(3)),stoi(ite->str(4)),stoi(ite->str(5)),stoi(ite->str(6)));
        ret_2 += get_cost(stoi(ite->str(1)),stoi(ite->str(2)),stoi(ite->str(3)),stoi(ite->str(4)),stoi(ite->str(5)),stoi(ite->str(6)), 10000000000000);
    }
    cout<< "Part 1: " << ret_1 << endl;   
    cout<< "Part 2: " << ret_2 << endl;  
    return 0;
}