#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::sqrt;
using std::map;

struct Card{
    char name;
    unsigned value;
    Card(const char& n);
};

bool operator==(Card& c1,Card& c2){return c1.value == c2.value;}
bool operator!=(Card& c1,Card& c2){return c1.value == c2.value;} 
bool operator<=(Card& c1,Card& c2){return c1.value <= c2.value;} 
bool operator>=(Card& c1,Card& c2){return c1.value >= c2.value;} 
bool operator<(Card& c1,Card& c2){return c1.value < c2.value;} 
bool operator>(Card& c1,Card& c2){return c1.value > c2.value;} 

Card::Card(const char& n):name(n){
    switch (name){
        case '2':
            value = 0;
            break;
        case '3':
            value = 1;
            break;
        case '4':
            value = 2;
            break;
        case '5':
            value = 3;
            break;
        case '6':
            value = 4;
            break;
        case '7':
            value = 5;
            break;
        case '8':
            value = 6;
            break;
        case '9':
            value = 7;
            break;
        case 'T':
            value = 8;
            break;
        case 'J':
            value = 9;
            break;
        case 'Q':
            value = 10;
            break;
        case 'K':
            value = 11;
            break;
        case 'A':
            value = 12;
            break;
    }
}
struct Hand{
    Hand() = delete;
    Hand(string& s,unsigned& bid);
    string value_string;
    vector<Card> value_cards;
    unsigned bid;
    map<char,unsigned> card_count;
    vector<unsigned> value_vector;
};

bool operator<(Hand& h1,Hand& h2){
    bool test_type = h1.value_vector == h2.value_vector;
    if(test_type){
        return h1.value_cards < h2.value_cards;
    } else{
        return h1.value_vector < h2.value_vector;
    }
} 

Hand::Hand(string& s,unsigned& bid):value_string(s),bid(bid),value_vector(5,0){
    for(const char& c:s){
        ++card_count[c];
        value_cards.emplace_back(c);
    }
    unsigned number_of_pair = 0;
    for( auto const& pair : card_count){
        //if( pair.second > value_vector[5]) value_vector[5] = pair.second;
        switch (pair.second){
            case 5:
                value_vector[0]+=1;
                break;
            case 4:
                value_vector[1]+=1;
                break;
            case 3:
                value_vector[3]+=1;
                break;
            case 2:
                value_vector[4]+=1;
                break;
        }
    }
    if(value_vector[3] > 0 && value_vector[4] > 0 )
        value_vector[2]+=1;
}

Hand process_line(string& line){
    string word;
    string s;
    unsigned b;
    istringstream record(line);
    record >> s;
    record >> b;
    return Hand(s,b);
}

int main(){
    std::ifstream file("test_input");
    string line;
    vector<Hand> hands;
    int res = 0;
    while(getline(file,line)){
        hands.push_back(process_line(line));
    }
    file.close();
    cout<< "T he total winnings are " << res << endl; 
    bool test = hands[0].value_vector < hands[4].value_vector;
    return 0;
}