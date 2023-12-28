#include<iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "pinfo.h"

using namespace std;

void Pinfo::printInfo() {
    int fd = open("/proc/self/stat", O_RDONLY);
    char data[1024];
    read(fd, data, 1024);
    char *pid = strtok(data, " ");
    char *executablePath = strtok(NULL, " ");
    const char *status = strtok(NULL, " ");
    if (pid == to_string(getpid())) {
        string as = status;
        status = (as + "+").c_str();
    }
    int i = 20;
    char *memory;
    while(i--) {
        memory = strtok(NULL, " ");
    }
    cout<<"pid -- "<<pid<<endl;
    cout<<"Process Status -- "<<status<<endl;
    cout<<"memory -- "<<memory<<endl;
    cout<<"Executable Path --"<<executablePath<<endl;
}

void Pinfo::printInfo(pid_t pid) {
    string path = "/proc/" + to_string(pid) + "/stat";
    int fd = open(path.c_str(), O_RDONLY);
    char data[1024];
    read(fd, data, 1024);
    strtok(data, " ");
    char *executablePath = strtok(NULL, " ");
    const char *status = strtok(NULL, " ");
    if (pid == getpid()) {
        string as = status;
        status = (as + "+").c_str();
    }
    int i = 20;
    char *memory;
    while(i-- && memory != NULL) {
        memory = strtok(NULL, " ");
    }
    cout<<"pid -- "<<pid<<endl;
    cout<<"Process Status -- "<<status<<endl;
    cout<<"memory -- "<<memory<<endl;
    cout<<"Executable Path --"<<executablePath<<endl;
}