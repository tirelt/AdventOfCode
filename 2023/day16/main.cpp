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
    void initiate(Head h){
        beam_heads.push_back(h);
    }
    void clear(){
        beam_heads.clear();
        for(vector<Tile>& line:tiles){
            for(Tile& t:line ){
                t = Tile(t.tile_value);
            }
        }
    }
    void propagate();
    int number_energized_tiles(){
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

int main(){
    
    // Part 1
    std::ifstream file_("test_input");
    Beams beams(file_);
    int res = 0;
    list<Head> entries;
    for(unsigned i=0;i<beams.tiles.size();++i){
        entries.emplace_back(i,-1,'>');
    }
    for(unsigned i=0;i<beams.tiles.size();++i){
        entries.emplace_back(i,beams.tiles[0].size(),'<');
    }
    for(unsigned i=0;i<beams.tiles[0].size();++i){
        entries.emplace_back(-1,i,'v');
    }
    for(unsigned i=0;i<beams.tiles[0].size();++i){
        entries.emplace_back(beams.tiles.size(),i,'^');
    }
    for(auto ite=entries.begin();ite!=entries.end();++ite){
        beams.clear();
        beams.initiate(*ite);
        while(beams.beam_heads.size()){
            beams.propagate();
        }
        auto temp = beams.number_energized_tiles();
        res = std::max(beams.number_energized_tiles(),res);
        cout << ite->hash() << " " << temp<< endl;
    }
    cout << "The number of energized tiles is: " << res << endl;
    return 0;
}