#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <utility>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::pair;
using std::map;

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

int main(){
    std::ifstream file("input");
    Beams beams(file);
    beams.initiate();
    while(beams.beam_heads.size()){
        beams.propagate();
    }
    cout << "The number of energized tiles is: " << beams.number_energized_tiles() << endl;
    return 0;
}