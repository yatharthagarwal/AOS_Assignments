#include<iostream>
#include<sys/types.h>
#include<dirent.h>
#include<vector>

using namespace std;

class ListFiles {
    public:
        void list(bool showHiddenFiles, bool longList, vector<string> path);
};