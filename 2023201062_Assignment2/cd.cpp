#include<iostream>
#include<unistd.h>
#include "cd.h"

using namespace std;

extern char* currentDirectory;
extern char* previousDirectory;
extern char* homeDirectory;

void ChangeCurrentDirectory::changeDirectory(char* input) {
    string inp = input;
    string currentDirectoryStr = currentDirectory;
    string homeDirectoryStr = homeDirectory;
    if (inp == ".." && currentDirectoryStr == homeDirectoryStr) {
        return;
    } else if (inp == "-") {
        char* changeTo = previousDirectory;
        previousDirectory = get_current_dir_name();
        chdir(changeTo);
        currentDirectory = get_current_dir_name();
    } else if (inp == "~") {
        previousDirectory = get_current_dir_name();
        chdir(homeDirectory);
        currentDirectory = get_current_dir_name();
    } else {
        previousDirectory = get_current_dir_name();
        chdir(input);
        currentDirectory = get_current_dir_name();
        if (inp == "..") {
            cout<<currentDirectory<<endl;
        }
    }
}   