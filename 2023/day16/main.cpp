#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::map;
using std::set;
using std::shared_ptr;
using std::make_shared;

struct Tile{
    Tile(char const& c):tile_value(c){
        directions['<'] = false;
        directions['^'] = false;
        directions['v'] = false;
        directions['>'] = false;
    }
    map<char,bool> directions;
    char tile_value;
    bool is_energized=false;
};

struct Head{
    Head(int k, int l, char d):i(k),j(l),direction(d){}
    Head(Head const& h):i(h.i),j(h.j),direction(h.direction){}
    int i;
    int j;
    char direction;
    int hash() const{
        int ret = i*1000000+j*1000;
        switch (direction)
        {
            case '<':
                ret += 1;
                break;
            case '^':
                ret += 2;
                break;
            case 'v':
                ret += 3;
                break;
            case '>':
                ret += 4;
                break;
        }
        return ret;
    }
};

Head reverse_hash(int hash){
    int r_1 = round(float(hash)/1000000);
    int r_2 =round(float(hash-r_1*1000000)/1000);
    int q = hash-r_1*1000000-r_2*1000;
    char c;
    switch (q)
        {
            case 1:
                c = '<';
                break;
            case 2:
                c = '^';
                break;
            case 3:
                c = 'v';
                break;
            case 4:
                c = '>';
                break;
        }
    return Head(r_1,r_2,c);     
}

struct Beams{
    Beams(std::ifstream& file);
    vector<vector<Tile>> tiles;
    list<Head> beam_heads;
    void initiate(){
        beam_heads.emplace_back(0,-1,'>');
    }
    void propagate();
    unsigned number_energized_tiles(){
        unsigned ret  = 0;
        for(auto line:tiles){
            for(auto t:line){
                if(t.is_energized) ret += 1;
            }
        }
        return ret;
    }
};

Beams::Beams(std::ifstream& file){
    string line;
    unsigned ret = 0;
    while(getline(file,line)){
        vector<Tile> tiles_line;
        for(char const& c : line){
            tiles_line.emplace_back(c);
        }
        tiles.push_back(tiles_line);
    }
    
}
void Beams::propagate(){
    Head h(beam_heads.front());
    beam_heads.pop_front();
    int i,j;
    switch (h.direction)
    {
        case '>':
            i = h.i;
            j = h.j+1;
            break;
        case '<':
            i = h.i;
            j = h.j-1;
            break;
        case '^':
            i = h.i-1;
            j = h.j;
            break;
        case 'v':
            i = h.i+1;
            j = h.j;
            break;
    }
    if(i>=0 && i<tiles.size() && j>=0 && j<tiles[0].size()){
        if(!tiles[i][j].directions[h.direction]){
            tiles[i][j].directions[h.direction] = true;
            tiles[i][j].is_energized = true;
            switch (tiles[i][j].tile_value)
            {
                case '.':
                    beam_heads.emplace_back(i,j,h.direction);
                    break;
                case '/':
                    switch (h.direction)
                    {
                        case '>':
                            beam_heads.emplace_back(i,j,'^');
                            break;
                        case '<':
                            beam_heads.emplace_back(i,j,'v');
                            break;
                        case '^':
                            beam_heads.emplace_back(i,j,'>');
                            break;
                        case 'v':
                            beam_heads.emplace_back(i,j,'<');
                            break;
                    }
                    break;
                case '\\':
                    switch (h.direction)
                    {
                        case '>':
                            beam_heads.emplace_back(i,j,'v');
                            break;
                        case '<':
                            beam_heads.emplace_back(i,j,'^');
                            break;
                        case '^':
                            beam_heads.emplace_back(i,j,'<');
                            break;
                        case 'v':
                            beam_heads.emplace_back(i,j,'>');
                            break;
                    }
                    break;
                case '|':
                    switch (h.direction)
                    {
                        case '<':
                        case '>':
                            beam_heads.emplace_back(i,j,'v');
                            beam_heads.emplace_back(i,j,'^');
                            break;
                        default:
                            beam_heads.emplace_back(i,j,h.direction);
                            break;
                    }
                    break;
                case '-':
                    switch (h.direction)
                    {
                        case '^':
                        case 'v':
                            beam_heads.emplace_back(i,j,'>');
                            beam_heads.emplace_back(i,j,'<');
                            break;
                        default:
                            beam_heads.emplace_back(i,j,h.direction);
                            break;
                    }
                    break;
            }
        }
    }
}

vector<char> process_line(string const& line){
    vector<char> line_vect;
    for(char const& c : line){
        line_vect.push_back(c);
    }
    return line_vect;
}

shared_ptr<vector<vector<bool>>> build_map_head(Head const& h,map<int,shared_ptr<vector<vector<bool>>>>& map_head, vector<vector<char>>& tiles,list<int> recursion_stack){
    int const hash = h.hash();
    if(hash==1003){
        auto test = 1;
    }
    vector<vector<bool>> energized(tiles.size(),vector<bool>(tiles[0].size(),false));
    auto ite_recursion = find(recursion_stack.begin(), recursion_stack.end(),hash);
    if(ite_recursion != recursion_stack.end()){
        for(unsigned k=0;k<tiles.size();++k){
            for(unsigned l=0;l<tiles[0].size();++l){
                for(auto ite = ite_recursion; ite != recursion_stack.end();++ite){
                    energized[k][l] = energized[k][l] || (*map_head[*ite])[k][l];
                }
            }
        }
        map_head[hash] = make_shared<vector<vector<bool>>>(energized);
        for(auto ite = ite_recursion; ite != recursion_stack.end();++ite){
            map_head[*ite] = map_head[hash];
        }
        return map_head[hash];
    }
    auto ite = map_head.find(hash);
    if(ite != map_head.end()){
        return ite->second;
    }
    int i,j;
    switch (h.direction)
    {
        case '>':
            i = h.i;
            j = h.j+1;
            break;
        case '<':
            i = h.i;
            j = h.j-1;
            break;
        case '^':
            i = h.i-1;
            j = h.j;
            break;
        case 'v':
            i = h.i+1;
            j = h.j;
            break;
    }
    list<Head> new_heads;
    map_head[hash] = make_shared<vector<vector<bool>>>(energized);
    recursion_stack.push_back(hash);
    if(i>=0 && i<tiles.size() && j>=0 && j<tiles[0].size()){
        (*map_head[hash])[i][j]=true;
        switch (tiles[i][j])
        {
            case '.':
                new_heads.emplace_back(i,j,h.direction);
                break;
            case '/':
                switch (h.direction)
                {
                    case '>':
                        new_heads.emplace_back(i,j,'^');
                        break;
                    case '<':
                        new_heads.emplace_back(i,j,'v');
                        break;
                    case '^':
                        new_heads.emplace_back(i,j,'>');
                        break;
                    case 'v':
                        new_heads.emplace_back(i,j,'<');
                        break;
                }
                break;
            case '\\':
                switch (h.direction)
                {
                    case '>':
                        new_heads.emplace_back(i,j,'v');
                        break;
                    case '<':
                        new_heads.emplace_back(i,j,'^');
                        break;
                    case '^':
                        new_heads.emplace_back(i,j,'<');
                        break;
                    case 'v':
                        new_heads.emplace_back(i,j,'>');
                        break;
                }
                break;
            case '|':
                switch (h.direction)
                {
                    case '<':
                    case '>':
                        new_heads.emplace_back(i,j,'v');
                        new_heads.emplace_back(i,j,'^');
                        break;
                    default:
                        new_heads.emplace_back(i,j,h.direction);
                        break;
                }
                break;
            case '-':
                switch (h.direction)
                {
                    case '^':
                    case 'v':
                        new_heads.emplace_back(i,j,'>');
                        new_heads.emplace_back(i,j,'<');
                        break;
                    default:
                        new_heads.emplace_back(i,j,h.direction);
                        break;
                }
                break;
        }    
        vector<shared_ptr<vector<vector<bool>>>> new_energizeds;
        for(Head const& g:new_heads){
            new_energizeds.push_back(build_map_head(g,map_head,tiles,recursion_stack));
        }
        for(unsigned k=0;k<tiles.size();++k){
            for(unsigned l=0;l<tiles.size();++l){
                for(shared_ptr<vector<vector<bool>>> const& e: new_energizeds){
                    (*map_head[hash])[k][l] = (*map_head[hash])[k][l] || (*e)[k][l];
                }
            }
        }
    }
    return map_head[hash];
}

unsigned count(shared_ptr<vector<vector<bool>>> const& matrix_ptr){
    unsigned ret = 0;
    for(vector<bool> const & line: *matrix_ptr){
        for(bool const& v:line){
            if(v) ret+=1;
        }
    }
    return ret;
}

map<int,std::pair<unsigned,list<Head>>> get_cycles(map<int,shared_ptr<vector<vector<bool>>>>& map_head){
    map<int,std::pair<unsigned,list<Head>>> cycles;
    vector<shared_ptr<vector<vector<bool>>>> known_ptrs;
    for(auto pair:map_head){
        auto ite = find(known_ptrs.begin(),known_ptrs.end(),pair.second);
        if(ite!=known_ptrs.end()){
            cycles[ite-known_ptrs.begin()].second.push_back(reverse_hash(pair.first));
        } else{
            cycles[ite-known_ptrs.begin()].second = {reverse_hash(pair.first)};
            cycles[ite-known_ptrs.begin()].first = count(pair.second);
            known_ptrs.push_back(pair.second);
        }
    }
    return cycles;
}

int main(){
    
    // Part 1
    std::ifstream file_("input");
    Beams beams(file_);
    file_.close();
    beams.initiate();
    while(beams.beam_heads.size()){
        beams.propagate();
    }
    //cout << "The number of energized tiles is: " << beams.number_energized_tiles() << endl;
    
    //Part 2 
    std::ifstream file("input");
    vector<vector<char>> tiles;
    string line;
    while(getline(file,line)){
        vector<char> tiles_line;
        for(char const& c : line){
            tiles_line.push_back(c);
        }
        tiles.push_back(tiles_line);
    }
    file.close();
    map<int,shared_ptr<vector<vector<bool>>>> map_head;
    list<int> recursion_stack;
    unsigned ret; 
    unsigned temp;
    {
    Head h(0,-1,'>');
    build_map_head(h, map_head, tiles, recursion_stack);
    temp = count(map_head[h.hash()]);
    }
    {
    Head h(-1,3,'v');
    build_map_head(h, map_head, tiles, recursion_stack);
    temp = count(map_head[h.hash()]);
    }
    {
    Head h(7,3,'>');
    build_map_head(h, map_head, tiles, recursion_stack);
    temp = count(map_head[h.hash()]);
    }
    auto cycles = get_cycles(map_head);
    for(unsigned i=0;i<tiles.size();++i){
        Head h(i,-1,'>');
        build_map_head(h, map_head, tiles, recursion_stack);
        temp = count(map_head[h.hash()]);
        cout << "The number of energized tiles is: " << temp << endl;
        ret = std::max(temp,ret);
    }
    for(unsigned i=0;i<tiles.size();++i){
        Head h(i,tiles[0].size(),'<');
        build_map_head(h, map_head, tiles, recursion_stack);
        temp = count(map_head[h.hash()]);
        cout << "The number of energized tiles is: " << temp << endl;
        ret = std::max(temp,ret);
    }
    for(unsigned i=0;i<tiles[0].size();++i){
        Head h(-1,i,'v');
        build_map_head(h, map_head, tiles, recursion_stack);
        temp = count(map_head[h.hash()]);
        cout << "The number of energized tiles is: " << temp << endl;
        ret = std::max(temp,ret);
    } 
    for(unsigned i=0;i<tiles[0].size();++i){
        Head h(tiles.size(),i,'^');
        build_map_head(h, map_head, tiles, recursion_stack);
        temp = count(map_head[h.hash()]);
        cout << "The number of energized tiles is: " << temp << endl;
        ret = std::max(temp,ret);
    }
    cout << "The max number of energized tiles is: " << ret << endl;
    return 0;
}