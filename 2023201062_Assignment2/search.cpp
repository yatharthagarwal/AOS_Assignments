#include<iostream>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include "search.h"

using namespace std;

bool Search::searchFile(string path, string file) {
    DIR *dir = opendir(path.c_str());
    dirent *dirent = readdir(dir);
    while(dirent != NULL) { 
        if (dirent->d_name[0] != '.') {
            if (file == dirent->d_name) {
                return true;
            } else {
                struct stat st;
                string nextPath = path + "/" + dirent->d_name;
                stat(nextPath.c_str(), &st);
                if (S_ISDIR(st.st_mode)) {
                    bool res = searchFile(nextPath, file);
                    if (res == true) return true;
                }
            }
        }
        dirent=readdir(dir);
    }
    return false;
}