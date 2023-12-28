#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<unistd.h>
#include "cwd.h"

using namespace std;

unordered_map<string, string> absolute_to_relative;
unordered_map<string, string> relative_to_absolute;

char* currentDirectory;
char* previousDirectory;
char* homeDirectory = HOME; 

string CurrentWorkingDirectory::getCWDPath() {
    string homeDir = homeDirectory;
    string currDir = currentDirectory;
    return "~" + currDir.substr(homeDir.length());
}

string CurrentWorkingDirectory::getAbsoluteCWDPath() {
    string relativePath = getCWDPath();
    return HOME + relativePath.substr(1);
}

void CurrentWorkingDirectory::initHomeDirectory() {
    chdir(HOME);
    previousDirectory = currentDirectory = get_current_dir_name();
    absolute_to_relative[homeDirectory] = "~";
    relative_to_absolute["~"] = homeDirectory;
}