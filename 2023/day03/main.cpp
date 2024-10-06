#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <math.h>
#include <map>
#include <utility>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::max;
using std::min;
using std::map;
using std::pair;

using ptr_matrix=shared_ptr<vector<vector<char>>>;
using ptr_map=shared_ptr<map<pair<unsigned int,unsigned int>,pair<unsigned int,unsigned int>>>;

ptr_matrix create_matrix(std::ifstream& file){
    string line;
    ptr_matrix matrix_ptr = make_shared<vector<vector<char>>>();
    vector<char> vect_line;
    int length=-1;
    while(getline(file,line)){
        if(line.size()!=length && length !=-1){
            throw std::runtime_error("Lines don't have the same length");
        } else {
            length = line.size();
        }
        vect_line.clear();
        for(const char c : line){
            vect_line.push_back(c);
        }
        matrix_ptr->push_back(vect_line);
    }
    return matrix_ptr;
}

   
bool check_arround(ptr_matrix& matrix_ptr,ptr_map& map_ptr, int i, int j, int k,int number_lines,int length,int n){
    int i_min = max(i-1,0),i_max = min(i+1,number_lines-1),j_min = max(j-1,0),j_max = min(k+1,length-1);
    for(int o = i_min;o<=i_max;++o){
        for(int p = j_min;p<=j_max;++p){
            const char& c = (*matrix_ptr)[o][p];
            if(c == '*'){
                auto ret = map_ptr->insert({{o,p},{1,n}});
                if(!ret.second){
                    ++ret.first->second.first;
                    ret.first->second.second*=n;
                }
            }
            if(!isdigit(c) && c != '.' ){
                return true;
            }
        }
    }
    return false;
}

int main(){
    std::ifstream file("input");
    ptr_matrix matrix_ptr = create_matrix(file);
    ptr_map map_ptr = make_shared<map<pair<unsigned int,unsigned int>,pair<unsigned int,unsigned int>>>();
    unsigned int sum = 0,sum_gear=0,j,k,n=0,length=(*matrix_ptr)[0].size(),number_lines=matrix_ptr->size();
    for(unsigned int i=0;i<number_lines;++i){
        j=0;
        vector<char>& line = (*matrix_ptr)[i];
        while(j<length){
            const char& c = line[j];
            if(isdigit(c)){
                k=j+1;
                while(k<length && isdigit(line[k])){
                    ++k;
                }
                n=0;
                for( unsigned int l = j;l<k;++l){
                    n += (line[l]-'0')*pow(10,k-l-1);
                }
                if(check_arround(matrix_ptr,map_ptr, i, j, k-1,number_lines,length,n)){
                    sum += n;
                }
                j=k+1;
            } else{
                ++j;
            }
        }
    }
    for(auto beg = map_ptr->begin();beg!=map_ptr->end();++beg){
        if(beg->second.first==2){
            sum_gear += beg->second.second;
        }
    }
    file.close();
    cout<< "The sum of the part numbers in the engine schematic is: "<< sum << endl; 
    cout<< "The sum of all of the gear ratios is : "<< sum_gear << endl; 
    return 0;
}