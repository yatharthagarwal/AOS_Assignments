#include<iostream>
#include<stack>
#include<unordered_map>
#include<vector>
#define HOME getenv("HOME")

using namespace std;

class CurrentWorkingDirectory {
    public:
        string getCWDPath(); // if true then absolute, else relative path.
        void initHomeDirectory();
        string getAbsoluteCWDPath();
};