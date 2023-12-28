#include<iostream>
#include <unistd.h>
#include "display.h"
#include "cwd.h"

int main() {
    Display dis;
    CurrentWorkingDirectory cwd;
    cwd.initHomeDirectory();        // initialize with /home/yatharth
    dis.display();
    return 0;
}