#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::map;

unsigned hash(string const& s){
    unsigned code = 0;
    for(char const& c : s){
        code += int(c);
        code *= 17;
        code = code % 256;
    }
    return code;
}

struct Lens{
    Lens(string l,unsigned f=0):label(l),focal(f){}
    unsigned focal;
    string label;
};

struct Box{
    unsigned number;
    list<Lens> lenses;
    void add_lens(Lens const& );
    void remove_lens(Lens const& );
    unsigned long power();
};

unsigned long Box::power(){
    unsigned long ret = 0;
    unsigned counter = 1;
    for(auto ite = lenses.begin();ite!=lenses.end();++ite){
        ret += (number+1)*(ite->focal)*counter++;
    }
    return ret;
}
void Box::add_lens(Lens const& lens){
    auto ite = lenses.begin();
    bool inserted = false;
    while(ite!=lenses.end() && !inserted){
        if(ite->label==lens.label){
            *ite = lens;
            inserted = true;
        }
        ++ite;
    }
    if(!inserted){
        lenses.insert(lenses.end(),lens);
    }
}

void Box::remove_lens(Lens const& lens){
    auto ite = lenses.begin();
    while(ite!=lenses.end()){
        if(ite->label==lens.label){
            lenses.erase(ite);
            break;
        }
        ++ite;
    }
}

void process_line(string const& line, map<unsigned,Box>& boxes){
    unsigned box_number;
    auto ite=line.begin();
    while(ite != line.end()&&*ite!='-'&&*ite!='='){
        ++ite;
    }
    string label(line.begin(),ite);
    box_number = hash(label);
    auto box_ite = boxes.find(box_number);
    if( box_ite != boxes.end()){
        if(*ite=='-'){
            Lens lens(label);
            box_ite->second.remove_lens(lens);
        }
        if(*ite=='='){
            string focal_str(++ite,line.end());
            int focal = stoi(focal_str);
            Lens lens(label,focal);
            box_ite->second.add_lens(lens);
        }
    } else{
        if(*ite=='='){
            string focal_str(++ite,line.end());
            int focal = stoi(focal_str);
            Lens lens(label,focal);
            Box b;
            b.add_lens(lens);
            b.number = box_number;
            boxes[box_number] = b;
            auto a =1;
        }
    }
}

int main(){
    std::ifstream file("input");
    string line;
    map<unsigned,Box> boxes;
    unsigned code;
    unsigned ret = 0;
    while(getline(file,line,',')){
        process_line(line,boxes);
    }
    for(auto box : boxes){
        ret += box.second.power();
    }
    file.close();
    cout << "The focusing power of the resulting lens configurationis " << ret << endl;
    return 0;
}