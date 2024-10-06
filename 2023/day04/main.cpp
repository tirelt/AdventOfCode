#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <map>
#include <set>
#include <memory>
#include <utility>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::find;
using std::map;
using std::set;
using std::make_shared;
using std::shared_ptr;
using std::pair;

using map_type = map<unsigned int,pair<unsigned int,bool>>;

int process_line(const string& line,shared_ptr<map_type>& number_cards){
    //cout << line;
    unsigned int i=5,j=0,counter=0,game_number,win_count,score;
    bool found_separator = false;
    vector<unsigned int> winning_numbers;
    vector<unsigned int> numbers;

    while(i<line.size()){
        const char& c = line[i];
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
            } else if(found_separator){
                numbers.push_back(n);
            } else{
                winning_numbers.push_back(n);
            }
            ++counter;
            i = j;
        } else if(c=='|'){
            found_separator = true;
            ++i;
        } else {
            ++i;
        }
    }
    auto ret = number_cards->insert({game_number,{1,true}});
    if(!ret.second){
        ++ret.first->second.first;
        ret.first->second.second=true;
    }
    win_count=0,score=0;
    for(const int& m:numbers){
        if(find(winning_numbers.begin(),winning_numbers.end(),m)!=winning_numbers.end()){
            if(win_count==0){
                score = 1;
            } else{
                score *= 2;
            }
            ++win_count;
        }
    }
    for(unsigned k=game_number+1;k<=game_number+win_count;++k){
        auto ret_add = number_cards->insert({k,{ret.first->second.first,false}});
        if(!ret_add.second){
            ret_add.first->second.first += ret.first->second.first;
        }
    }
    return score;
}

int main(){
    std::ifstream file("input");
    string line;
    shared_ptr<map_type> number_cards = make_shared<map_type>();
    unsigned int sum = 0,number_cards_sum = 0;
    while(getline(file,line)){
        sum += process_line(line,number_cards);
    }
    for(auto beg = number_cards->begin();beg!=number_cards->end();++beg){
        if(beg->second.second){
            number_cards_sum+=beg->second.first;
        }
    }
    file.close();
    cout<< "The sum ot the points of the cards is: "<< sum << endl; 
    cout<< "The snumber of cards is: "<< number_cards_sum << endl; 
    return 0;
}