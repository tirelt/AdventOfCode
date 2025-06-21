#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>


using std::string;
using std::cout;
using std::endl;
using std::map;
using std::set;

struct File {
    File(string n, int s) : name(n), size(s) {}
    string name;
    int size;
};

struct Directory {
    Directory(string n) : name(n) {}
    string name;
    int size;
    set<File*> files;
    set<Directory*> subdirs;
    Directory* parent = nullptr;
    void get_size() {
        int total_size = 0;
        for(Directory* subdir : subdirs) {
            subdir->get_size();
            total_size += subdir->size;
        }
        for(const File* file : files) {
            total_size += file->size;
        }
        size = total_size;
    }
};

int main(){
    std::ifstream file("input");
    string line;
    int res_1 = 0;
    Directory* current_dir = new Directory{"/"};
    map<string, Directory*> directories = {{current_dir->name, current_dir}};
    string prev_cmd;
    while(getline(file,line)){
        if(line[0] == '$') {
            prev_cmd = line.substr(2,2);
            if(line.substr(2, 2) == "cd") {
                if(line.substr(5)== "..") {
                    current_dir = current_dir->parent;
                } else {
                    if(line.substr(5) == "/") {
                        current_dir = directories["/"];
                    } else {
                        string dir_name = current_dir->name + line.substr(5) + '/';
                        if(directories.find(dir_name) == directories.end()){
                            directories[dir_name] = new Directory{dir_name};
                            directories[dir_name] -> parent = current_dir;
                            current_dir->subdirs.insert(directories[dir_name]);
                        }
                        current_dir = directories[dir_name];
                    }
                }
            } else if(line.substr(2, 2) == "ls") {
                continue; 
            } else {
                throw std::runtime_error("Unknown command: " + line);
            }
        } else {
            if(prev_cmd == "ls") {
                if(line.substr(0, 3) == "dir") {
                    string dir_name = current_dir->name + line.substr(4) + '/';
                    if(directories.find(dir_name) == directories.end()){
                        directories[dir_name] = new Directory{dir_name};
                        directories[dir_name]->parent = current_dir;
                        current_dir->subdirs.insert(directories[dir_name]);
                    }
                } else {
                    int space_pos = line.find(' ');
                    int size = std::stoi(line.substr(0, space_pos));
                    string file_name = line.substr( space_pos+ 1);
                    File* file = new File{file_name, size};
                    current_dir->files.insert(file);
                }
            } else {
                throw std::runtime_error("Should not go here");
            }
        }
    }
    file.close();
    directories["/"]->get_size();
    int cutoff = 30000000 - (70000000 - directories["/"]->size);
    Directory* smallest_dir = directories["/"];
    for(const auto& [name, dir] : directories) {
        if(dir->size <= 100000) {
            res_1 += dir->size;
        }
        if (dir->size >= cutoff && dir->size < smallest_dir->size) {
            smallest_dir = dir;
        }
    }
    cout<< "Part 1: " << res_1 << endl; 
    cout<< "Part 2: " << smallest_dir->size << endl; 
    return 0;
}