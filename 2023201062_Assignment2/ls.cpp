#include<iostream>
#include<sys/stat.h>
#include<pwd.h>
#include<cstdio>
#include<time.h>
#include<math.h>
#include<grp.h>
#include "ls.h"
#include "cwd.h"

using namespace std;

extern char* homeDirectory;

string getPermissions(string filename, mode_t  mode) {
    string permissions;
    permissions += ((S_ISDIR(mode)) ? 'd' : '-');
    permissions += ((mode & S_IRUSR) == S_IRUSR) ? "r" : "-";
    permissions += ((mode & S_IWUSR) == S_IWUSR) ? "w" : "-";
    permissions += ((mode & S_IXUSR) == S_IXUSR) ? "x" : "-";

    permissions += ((mode & S_IRGRP) == S_IRGRP) ? "r" : "-";
    permissions += ((mode & S_IWGRP) == S_IWGRP) ? "w" : "-";
    permissions += ((mode & S_IXGRP) == S_IXGRP) ? "x" : "-";

    permissions += ((mode & S_IROTH) == S_IROTH) ? "r" : "-";
    permissions += ((mode & S_IWGRP) == S_IWGRP) ? "w" : "-";
    permissions += ((mode & S_IXOTH) == S_IXOTH) ? "x" : "-";
    return permissions;
}

string getNameById(long long userId) {
    struct passwd *pd = getpwuid(userId);
    if (pd == NULL) {
        return "";
    }
    string name = pd->pw_name;
    return name;
} 

string getLongList(string filename, struct stat &st) {
    string permissions = getPermissions(filename, st.st_mode);
    unsigned long linkCount = st.st_nlink;
    string userName = getNameById(st.st_uid);
    string groupName = getNameById(st.st_gid);
    __off_t fileSize = st.st_size; 
    struct timespec fileModificationTime = st.st_mtim;
    char res[99];
    char buffer[15];
    strftime(buffer,15,"%b  %d %H:%M", gmtime(&fileModificationTime.tv_sec));
    sprintf(res, "%10s %4lu %8s %8s %15ld %15s %30s", permissions.c_str(), linkCount, userName.c_str(), groupName.c_str(), fileSize, buffer, filename.c_str());
    return res;
}

string getAbsolutePath(string path) {
    string home = homeDirectory;
    CurrentWorkingDirectory cwd;
    string absolutePath;
    if (path.length() == 0) {
        absolutePath = HOME + cwd.getCWDPath().substr(1);
    } else if (path[0] == '.') {
        absolutePath = HOME + cwd.getCWDPath().substr(1) + path.substr(1);
    } else if (home == path.substr(0, home.length())) {
        absolutePath = path;
    } 
    else if (isalpha(path[0]) != 0) {
        absolutePath = HOME + cwd.getCWDPath().substr(1) + "/" + path;
    }
    return absolutePath;
}

void ListFiles::list(bool showHiddenFiles, bool longList, vector<string> path) {
    for(int i=0; i<path.size(); i++) {
        vector<string> longlists;
        if (path.size() > 1 && path[i].length() > 0 && path[i][0] != '-') {
            cout<<path[i]<<":"<<endl;
        }
        string absolutePath = getAbsolutePath(path[i]);
        DIR *dir = opendir(absolutePath.c_str());
        if (dir == NULL) {
            perror("No such Directory");
            return;
        }
        dirent *dirent = readdir(dir);
        struct stat dirSt;
        stat(absolutePath.c_str(), &dirSt);
        while(dirent != NULL) {
            if (showHiddenFiles) {
                if (longList) {
                    struct stat st;
                    stat((absolutePath+"/"+dirent->d_name).c_str(), &st);
                    longlists.push_back(getLongList(dirent->d_name, st));
                } else {
                    printf("%s ", dirent->d_name);
                }
            } else {
                if (dirent->d_name[0] != '.' ) {
                    if (longList) {
                        struct stat st;
                        stat((absolutePath+"/"+dirent->d_name).c_str(), &st);
                        longlists.push_back(getLongList(dirent->d_name, st));
                    } else {
                        printf("%s ", dirent->d_name);
                    }
                }
            }
            dirent = readdir(dir);
        }
        if (longlists.size() > 0) {
            for(int j=0; j<longlists.size(); j++) {
                cout<<longlists[j]<<endl;
            }
        }
        closedir(dir);
        cout<<endl;
    }
}