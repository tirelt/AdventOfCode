# Advent Of Code

Solving [Advent Of Code](https://adventofcode.com/2023/) problems in C++.

# Usage

Use the `makefile` in the problem subfolders.
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
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug 2024/day12/main",
            "type": "lldb",
            "console": "integratedTerminal",
            "request": "launch",
            "program": "${cwd}/2024/day12/main",
            "args": [],
            "cwd": "${cwd}/2024/day12",
            "preLaunchTask": "Build 2024/day12/main"
        }

    ]
}
```
