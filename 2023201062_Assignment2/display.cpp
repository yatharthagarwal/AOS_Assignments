#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include <algorithm>
#include <cctype>
#include<vector>

#include "display.h"
#include "echo.h"
#include "ls.h"
#include "cd.h"
#include "cwd.h"
#include "process.h"
#include "pinfo.h"
#include "search.h"

using namespace std;

extern char* currentDirectory;
extern char* previousDirectory;

string Display::getUserAndHostName() {
    string username = getlogin();
    char charHostname[50];
    if (gethostname(charHostname, 50) == -1) {               // get hostname upto 30 chars length
        perror("Error while getting hostname of the system");
        return username;
    }
    string hostname = charHostname;
    string s = username + "@" + hostname;
    return s;
}

string Display::getCurrentWorkingDirectory() {
    CurrentWorkingDirectory cwd;
    string cwdPath = cwd.getCWDPath();
    string notation = ":" + (cwdPath.length() > 0 ? cwdPath : "") + ">";
    return notation;
}

vector<string> collectArgs(char* arg) {
    char *c = strtok(arg, " ");
    vector<string> args;
    while(c != NULL) {
        string strArg = c;
        args.push_back(strArg);
        c = strtok(NULL, " ");
    }
    return args;
}

void Display::display() {
    string host = Display::getUserAndHostName();
    string cwd = currentDirectory;
    string input_string;
    CurrentWorkingDirectory currentWorkingDirectory;
    while(1) {
        string ans = host + ":" + currentWorkingDirectory.getCWDPath() + ">";
        cout<< ans;
        ans.erase(remove_if(ans.begin(), ans.end(), ::isspace), ans.end());
        getline(cin, input_string);
        char buffer_input[1024];
        strcpy(buffer_input, input_string.c_str());
        string command = strtok(buffer_input, " ");
        char *arg = strtok(NULL, "");
        vector<string> args;
        if (arg != NULL) {
            char *temp_arg = arg;
            char *c = strtok(temp_arg, " ");
            while(c != NULL) {
                string strArg = c;
                args.push_back(strArg);
                c = strtok(NULL, " ");
            }
        }

        if (args.size() > 0 && strcmp(args.back().c_str(), "&") == 0) {
            char *arguments[args.size()];
            int i;
            for(i=0; i<args.size()-1; i++) {
                strcpy(arguments[i], args[i].c_str());
            }
            arguments[i] = (char *)NULL;
            Process process;
            process.forkBackgroundProcess(command.c_str(), arguments);
        } else if (command == "echo") {
            Echo ec;
            ec.echo(arg);
        } else if (command == "pwd") {
            cout<<currentWorkingDirectory.getAbsoluteCWDPath()<<endl;
        } else if (command == "ls") {
            struct FLAGS {
                bool showHiddenFiles;
                bool longList;
                vector<string> path;
            };
            struct FLAGS flag;
            flag.longList = false;
            flag.showHiddenFiles = false;
            if (args.size() > 0) {
                for(int i=0; i<(int)args.size(); i++) {
                    if (args[i][0] == '-') {
                        for(int j=1; args[i][j] != '\0'; j++) {
                            if (args[i][j] == 'a') {
                                flag.showHiddenFiles = true;
                            }
                            if (args[i][j] == 'l') {
                                flag.longList = true;
                            }
                        }
                    } else if (args[i] == "..") {
                        (flag.path).push_back(previousDirectory);
                    } else if (args[i][0] == '~') {
                        // TODO
                        (flag.path).push_back(HOME + args[i].substr(1));
                    } else if (isalpha(args[i][0]) != 0 || args[i][0] == '.') {
                        (flag.path ).push_back(args[i]);
                    }
                }
            } 
            if ((flag.path).size() == 0) {
                (flag.path).push_back("");
            }
            ListFiles listFiles;
            listFiles.list(flag.showHiddenFiles, flag.longList, flag.path);
        } else if (command == "cd") {
            ChangeCurrentDirectory ccd;
            if (arg != NULL) {
                ccd.changeDirectory(arg);
            }
        } else if (command == "pinfo") {
            Pinfo pinfo;
            if (args.size() > 0) {
                pinfo.printInfo(stoi(args[0]));
            } else {
                pinfo.printInfo();
            }
        } else if (command == "search") {
            Search search;
            string file = arg;
            string path = get_current_dir_name();
            cout<< boolalpha <<search.searchFile(path, file)<<endl;
        }
    }
}