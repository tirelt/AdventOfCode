#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::map;
using std::set;


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
    unsigned hash() const{
        unsigned ret = i*1000000+j*100;
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
    file.close();
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

vector<vector<bool>> build_map_head(Head const& h,map<unsigned,vector<vector<bool>>>& map_head, vector<vector<char>>& tiles,set<unsigned> recursion_stack){
    unsigned const hash = h.hash();
    auto ite = map_head.find(hash);
    if(ite != map_head.end()){
        return ite->second;
    }
    vector<vector<bool>> energized(tiles.size(),vector<bool>(tiles[0].size(),false));
    if (recursion_stack.find(hash) != recursion_stack.end()){
        map_head[hash]=energized;
        return energized;
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
    if(i>=0 && i<tiles.size() && j>=0 && j<tiles[0].size()){
        energized[i][j]=true;
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
        vector<vector<vector<bool>>> new_energizeds;
        recursion_stack.insert(hash);
        for(Head const& g:new_heads){
            new_energizeds.push_back(build_map_head(g,map_head,tiles,recursion_stack));
        }
        for(unsigned k=0;k<tiles.size();++k){
            for(unsigned l=0;l<tiles.size();++l){
                for(vector<vector<bool>> const& e:new_energizeds){
                    energized[k][l] = energized[k][l] || e[k][l];
                }
            }
        }
    }
    map_head[hash]=energized;
    return energized;
}
int main(){
    std::ifstream file("input");
    // Part 1
    Beams beams(file);
    beams.initiate();
    while(beams.beam_heads.size()){
        beams.propagate();
    }
    cout << "The number of energized tiles is: " << beams.number_energized_tiles() << endl;
    //Part 2 
    Head h(0,-1,'>');
    map<unsigned,vector<vector<bool>>> map_head;
    vector<vector<char>> tiles;
    set<unsigned> recursion_stack;
    build_map_head(h, map_head, tiles, recursion_stack);
    return 0;
}