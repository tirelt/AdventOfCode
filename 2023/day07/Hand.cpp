#include <Hand.h>
#include <string>
#include <sstream>

using std::string;
using std::istringstream;


bool operator<(const Hand& h1,const Hand& h2){
    bool test_type = h1.value_vector == h2.value_vector;
    if(test_type){
        return h1.cards < h2.cards;
    } else{
        return h1.value_vector < h2.value_vector;
    }
   return true;
} 
bool operator==(const Hand& h1,const Hand& h2){
   return h1.value_string==h2.value_string;
} 

Hand::Hand(string& s,unsigned& bid):value_string(s),bid(bid),value_vector(5,0){
    for(const char& c:s){
        ++card_count[c];
        cards.emplace_back(c);
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