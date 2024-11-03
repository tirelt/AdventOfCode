#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <Card.h>
#include <Hand.h>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::sort;

int main(){
    std::ifstream file("input");
    string line;
    vector<Hand> hands;
    int res = 0;
    while(getline(file,line)){
        hands.push_back(process_line(line));
    }
    file.close();
    sort(hands.begin(),hands.end());
    unsigned rank = 1;
    for(auto ite = hands.begin(); ite < hands.end();++ite){
        res += ite->bid * rank++;
    }
    cout<< "The total winnings are " << res << endl; 
    return 0;
}