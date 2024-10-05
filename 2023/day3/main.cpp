#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <math.h>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::max;
using std::min;

using ptr_type=shared_ptr<vector<vector<const char>>>;

ptr_type create_matrix(std::ifstream& file){
    string line;
    auto matrix_ptr = make_shared<vector<vector<const char>>>();
    vector<const char> vect_line;
    int length=-1;
    while(getline(file,line)){
        if(line.size()!=length && length ==-1){
            throw std::runtime_error("Lines don't have the same length");
        } else {
            length = line.size();
        }
        for(const char& c : line){
            vect_line.push_back(c);
        }
        matrix_ptr->push_back(vect_line);
    }
    return matrix_ptr;
}
bool check_arround(ptr_type matrix_ptr, int i, int j, int k){
    int length=(*matrix_ptr)[0].size(),number_lines = matrix_ptr->size();
    int i_min = max(i-1,0),i_max = min(i+1,number_lines-1),j_min = max(j-1,0),j_max = min(k+1,length-1);
    bool include = false;
    for(int o = i_min;o<=i_max;++o){
        for(int p = j_min;p<=j_max;++p){
            
        }
    }
}

int main(){
    std::ifstream file("input");
    ptr_type matrix_ptr = create_matrix(file);
    unsigned int sum = 0,j,k,n,length=(*matrix_ptr)[0].size();
    vector<const char> line;
    for(unsigned int i=0;i<matrix_ptr->size();++i){
        j=0;
        line = (*matrix_ptr)[i];
        while(j<length){
            const char c = line[j];
            if(isdigit(c)){
                k=j+1;
                while(k<length && isdigit(line[k])){
                    ++k;
                }
                for( unsigned int l = j;l<k;++l){
                    n += (line[l]-'0')*pow(10,k-l-1);
                }

                j=k+1;
            } else{
                ++j;
            }
        }
    }
    file.close();
    cout<< "The sum of the part numbers in the engine schematic is: "<< sum << endl; 
    return 0;
}