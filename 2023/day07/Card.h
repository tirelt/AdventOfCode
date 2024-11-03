#ifndef CARD_H
#define CARD_H

struct Card{
    char name;
    unsigned value;
    Card(const char& n);
};

bool operator==(const Card& c1,const Card& c2);
bool operator!=(const Card& c1,const Card& c2);
bool operator<=(const Card& c1,const Card& c2);
bool operator>=(const Card& c1,const Card& c2);
bool operator<(const Card& c1,const Card& c2);
bool operator>(const Card& c1,const Card& c2);
#endif