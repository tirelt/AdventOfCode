#ifndef HAND_H
#define HAND_H

#include <string>
#include <vector>
#include <map>
#include <Card.h>

struct Hand{
    Hand() = delete;
    Hand(std::string& s,unsigned& bid);
    std::string value_string;
    std::vector<Card> cards;
    unsigned bid;
    std::map<char,unsigned> card_count;
    std::vector<unsigned> value_vector;
};

bool operator<(const Hand& h1,const Hand& h2);
Hand process_line(std::string& line);

#endif
