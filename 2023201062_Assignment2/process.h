#include<iostream>

using namespace std;

class Process {
    public:
        void forkBackgroundProcess(const char* command, char *argv[]);
};