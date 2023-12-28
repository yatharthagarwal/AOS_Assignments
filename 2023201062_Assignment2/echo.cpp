#include<iostream>
#include<string.h>
#include<string>
#include <fcntl.h>
#include <unistd.h>
#include "echo.h"

using namespace std;

void Echo::echo(char* inp) {
    string input = inp;
    int pos = input.rfind(">>");
    if (pos != string::npos) {
        char* text = strtok(inp, ">>");
        string str = text;
        int len = str.length();
        const char* fileName =  input.substr(pos + 2).c_str(); // strtok(NULL, "");
        int fd = open(fileName, O_RDWR|O_APPEND);
        if (fd == -1) {
            perror("No such file exists");
        } else {
            write(fd, text, len);
        }
    } else if (input.find('>') != string::npos) {
        char* text = strtok(inp, ">");
        string str = text;
        int len = str.length();
        char* fileName = strtok(NULL, "");
        int fd = open(fileName, O_RDWR|O_TRUNC);
        if (fd == -1) {
            perror("No such file exists");
        } else {
            write(fd, text, len);
        }
    } else  {
        string text = "";
        for (int i = 0; i < input.size(); i++) {
            text += input[i];
        }
        cout<<text<<endl;
    }
}