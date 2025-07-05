#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::sort;

bool isInteger(const string& s) {
    for (const char& c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return !s.empty();
}

int findFirst(const string& s) {
    int i = 0,count = 0;
    while( i < s.size() && (count > 0 || (s[i] != ']')&& (s[i] != ','))) {
        if(s[i] == '[')
            ++count;
        else if(s[i] == ']')
            --count;
        ++i;
    }
    return i;
}
bool isOrdered(const string& left, const string& right) {
    if(isInteger(left) && isInteger(right)) {
        return std::stoi(left) < std::stoi(right);
    } else if(isInteger(left)){
        const string intertolist = "[" + left + "]";
        return isOrdered(intertolist, right); 
    } else if(isInteger(right)){
        const string intertolist = "[" + right + "]";
        return isOrdered(left, intertolist); 
    } else {
        if(right.size() == 0 && left.size() == 0)
            return true;
        else if(right.size() == 2)
            return false; 
        else if(left.size() == 2)
            return true; 
        else {
            int i = findFirst(left.substr(1));
            int j = findFirst(right.substr(1));
            const string leftFirst = left.substr(1, i);
            const string rightFirst = right.substr(1, j);
            const string leftRest= "[" + left.substr(i + 1 + (left[i+1] == ','? 1 : 0));
            const string rightRest= "[" + right.substr(j + 1 + (right[j+1] == ','? 1 : 0));

            if(isOrdered(leftFirst, rightFirst))
                return true;
            else if(leftFirst == rightFirst)
                return isOrdered(leftRest, rightRest);
            else 
                return false;
        }
    }
}

int main(){
    std::ifstream file("input");
    string line;
    int res_1 = 0;
    int i = 0, count = 0;
    vector<string> packets(2);
    vector<string> decoder_packets{"[[2]]", "[[6]]"}; // divider packets for part 2
    vector<string> all_packets(decoder_packets); // all packets for part 2, including the two divider packets
    while(getline(file,line)){
        if(line.empty()){ 
            i = 0;
            ++count;
            if(isOrdered(packets[0], packets[1]))
                res_1 += count;
            continue;
        } 
        packets[i++] = line;
        all_packets.push_back(line);
    }
    if(isOrdered(packets[0], packets[1]))
        res_1 += ++count;
    file.close();
    
    cout<< "Part 1: " << res_1 << endl; 
    
    // Part 2
    int res_2 = 1;
    sort(all_packets.begin(), all_packets.end(), isOrdered);
    for(const string& packet : decoder_packets) {
        auto it = std::find(all_packets.begin(), all_packets.end(), packet);
        res_2 *=  it - all_packets.begin() + 1;
    }
    cout<< "Part 2: " << res_2 << endl; 

    return 0;
}