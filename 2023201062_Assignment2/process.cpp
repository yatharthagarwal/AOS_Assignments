#include<iostream>
#include<string.h>
#include<unistd.h>
#include "process.h"

using namespace std;

int totalChildrenProcess = 0;

void Process::forkBackgroundProcess(const char* command, char *argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("No child process created");
    } else if (pid > 0) {
        printf("[%d]  %d\n", ++totalChildrenProcess, pid);
    } else if (pid == 0) {
        string cmd = command;
        string path = "/usr/bin/" + cmd;
        execvp(path.c_str(), argv);
    }
}