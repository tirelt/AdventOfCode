#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using std::string;
using std::cout;
using std::endl;
using std::vector;

int process_line(const string& line,unsigned int max_r,unsigned int max_g,unsigned int max_b){
    cout << line;
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
        cout << " possible" << endl;
        return game_number;
    } else{
        cout << " imppossible" << endl;
        return 0;
    }
}


int main(){
    std::ifstream file("input");
    string line;
    int sum = 0;
    while(getline(file,line)){
        sum += process_line(line,12,13,14);
    }
    file.close();
    cout<< "The sum of the IDs is: "<< sum << endl; 
    return 0;
}