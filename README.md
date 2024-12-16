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
    "label": "Build [year]/[day]/main",
    "command": "make",
    "args": ["main"],
    "options": {
        "cwd": "${cwd}/[year]/[day]"
    },
    "detail": "Build [year]/[day]/main"
}
```
```bash
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug [year]/[day]/main",
            "type": "lldb",
            "console": "integratedTerminal",
            "request": "launch",
            "program": "${cwd}/[year]/[day]/main",
            "args": [],
            "cwd": "${cwd}/[year]/[day]",
            "preLaunchTask": "Build [year]/[day]/main"
        }

    ]
}
```
