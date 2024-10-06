# Advent Of Code

Trying to solve [Advent Of Code](https://adventofcode.com/2023/) problems in C++ 

# Usage

Use the 'makefile' in the problem subfolders.
e.g.
```bash
make #to build the project
make run #to run the program
make clean # to delete object files and the program
```

# Debug

Using `VSCode` change `.vscode/tasks.json` and `.vscode/launch.json` as per below 
```bash
{
    "type": "cppbuild",
    "label": "Build [path_to_folder]/main",
    "command": "make",
    "args": ["main"],
    "options": {
        "cwd": "${cwd}/[path_to_folder]"
    },
    "detail": "Build [path_to_folder]/main"
}
```
```bash
{
    "name": "Debug [path_to_folder]/main",
    "type": "cppdbg",
    "request": "launch",
    "program": "${cwd}/[path_to_folder]/main",
    "args": [],
    "stopAtEntry": false,
    "cwd": "${cwd}/[path_to_folder]",
    "environment": [],
    "externalConsole": false,
    "MIMode": "gdb",
    "miDebuggerPath": "/usr/bin/gdb",
    "setupCommands": [
        {
        "description": "Enable pretty-printing for gdb",
        "text": "-enable-pretty-printing",
        "ignoreFailures": true
        }
    ],
    "preLaunchTask": "Build [path_to_folder]/main"
}
```
