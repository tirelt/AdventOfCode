#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::vector;
using std::map;
using std::set;
using std::pair;

struct Secret{
    Secret(long long k):n(new long long(k)){}
    ~Secret(){
        delete n;
    }
    long long* n;
    long long& value(){
        return *n;
    }
    long long operator*(long long mult){
        return *n * mult;
    }
    long long operator/(long long div){
        return *n / div;
    }
    Secret& mix(long long input){
        *n = input ^ *n; 
        return *this;
    }
    Secret& prune(int pruner = 16'777'216){
        *n = *n % pruner; 
        return *this;
    }
    Secret& next(){
        (*this).mix(*this * 64).prune();
        (*this).mix(*this / 32).prune();
        (*this).mix(*this * 2048).prune();
        return *this;
    }
};

int hash(int a,int b, int c, int d){
    int ret = ((a>=0)?10'000'000:0) + 1'000'000* abs(a) + ((b>=0)?100'000:0) + 10'000 * abs(b) + ((c>=0)?1'000:0)+ 100 * abs(c) + ((d>=0)?10:0)+  abs(d);
    return ret;
}

int main(){
    std::ifstream file("input");
    string line;
    
    long long ret_1 = 0;
    vector<vector<int>> prices;
    vector<int> temp;

    while(getline(file,line)){
        Secret secret(stoi(line));
        temp.clear();
        temp.push_back(stoi(line));
        for(int i = 0;i<2000;++i){
            secret.next();
            temp.push_back(secret.value()%10);
        }
        prices.push_back(temp);
        //cout << secret.value() << endl;
        ret_1 += secret.value();
    }
    cout << "Part 1: " << ret_1 << endl;

    map<int,pair<int,set<int>>> memo;
    int ret_2;
    for(int i=0;i<prices.size();++i){
        for(int j = 4; j<prices[0].size();++j){
            int h = hash(prices[i][j-3]-prices[i][j-4],prices[i][j-2]-prices[i][j-3],prices[i][j-1]-prices[i][j-2],prices[i][j]-prices[i][j-1]);
            const auto ret_insert = memo[h].second.insert(i);
            if(ret_insert.second)
                 memo[h].first += prices[i][j];
        }
    }
    int key;
    for(const auto& p:memo){
        if(p.second.first>ret_2){
            ret_2 = p.second.first;
            key = p.first;
        }
    }
    cout << "Part 2: " << ret_2 << endl;
    return 0;
}