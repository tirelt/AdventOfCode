#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using std::string;
using std::cout;
using std::endl;
using std::max;

int process_line(const string& line,unsigned int max_r,unsigned int max_g,unsigned int max_b){
    //cout << line;
    unsigned int i=5,j=0,counter=0,game_number;
    bool possible_game = true;
    while(i<line.size()){
        const char c = line[i];
        if(isdigit(c)){
            j = i + 1;
            while(j<line.size() && isdigit(line[j])){
                ++j;
            }       
            unsigned int n = 0;
            for( unsigned int l = i;l<j;++l){
                n += (line[l]-'0')*pow(10,j-l-1);
            }
            if(counter==0){
                game_number = n;
            } else {
                if( line[j+1] == 'r'){
                    possible_game = possible_game && (n<=max_r);
                }
                if( line[j+1] == 'g'){
                    possible_game = possible_game && (n<=max_g);
                }
                if( line[j+1] == 'b'){
                    possible_game = possible_game && (n<=max_b);
                }
            }
            ++counter;
        }
        ++i;
    }
    if(possible_game){
        //cout << " possible" << endl;
        return game_number;
    } else{
        //cout << " imppossible" << endl;
        return 0;
    }
}

int power_line(const string& line){
    //cout << line;
    unsigned int i=5,j=0,counter=0,game_number,max_r=0,max_g=0,max_b=0;
    while(i<line.size()){
        const char c = line[i];
        if(isdigit(c)){
            j = i + 1;
            while(j<line.size() && isdigit(line[j])){
                ++j;
            }       
            unsigned int n = 0;
            for( unsigned int l = i;l<j;++l){
                n += (line[l]-'0')*pow(10,j-l-1);
            }
            if(counter==0){
                game_number = n;
            } else {
                if( line[j+1] == 'r'){
                    max_r = max(max_r,n);
                }
                if( line[j+1] == 'g'){
                    max_g = max(max_g,n);
                }
                if( line[j+1] == 'b'){
                    max_b = max(max_b,n);
                }
            }
            ++counter;
        }
        ++i;
    }
    unsigned int power = max_r*max_g*max_b;
    //cout << " " << power << endl;
    return power;
}


int main(){
    std::ifstream file("input");
    string line;
    unsigned int sum = 0,power_sum = 0;

    while(getline(file,line)){
        sum += process_line(line,12,13,14);
        power_sum += power_line(line);
    }
    file.close();
    cout<< "The sum of the IDs is: "<< sum << endl; 
    cout<< "The sum of the power is: "<< power_sum << endl; 
    return 0;
}