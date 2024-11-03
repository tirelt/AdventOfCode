#include <Card.h>

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

bool operator==(const Card& c1,const Card& c2){return c1.value == c2.value;}
bool operator!=(const Card& c1,const Card& c2){return c1.value == c2.value;} 
bool operator<=(const Card& c1,const Card& c2){return c1.value <= c2.value;} 
bool operator>=(const Card& c1,const Card& c2){return c1.value >= c2.value;} 
bool operator<(const Card& c1,const Card& c2){return c1.value < c2.value;} 
bool operator>(const Card& c1,const Card& c2){return c1.value > c2.value;} 