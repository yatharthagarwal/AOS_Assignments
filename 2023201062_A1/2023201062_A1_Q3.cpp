#include <sys/stat.h>
#include <unistd.h>
#include<fcntl.h>
#include<iostream>
using namespace std;

const int BATCH_SIZE = 65536;

char ans[][4] = { "No", "Yes" };
char typeOfFile[][10] = { "oldfile", "newfile", "directory" };

void getInformation(char name[], int type) {
    struct stat st;
    stat(name, &st);
    cout<<"User has read permissions on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IRUSR) == S_IRUSR)]<<endl;
    cout<<"User has write permission on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IWUSR) == S_IWUSR)]<<endl;
    cout<<"User has execute permission on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IXUSR) == S_IXUSR)]<<endl;
    cout<<"Group has read permissions on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IRGRP) == S_IRGRP)]<<endl;
    cout<<"Group has write permission on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IWGRP) == S_IWGRP)]<<endl;
    cout<<"Group has execute permission on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IXGRP) == S_IXGRP)]<<endl;
    cout<<"Others has read permissions on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IROTH) == S_IROTH)]<<endl;
    cout<<"Others has write permission on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IWOTH) == S_IWOTH)]<<endl;
    cout<<"Others has execute permission on "<< typeOfFile[type]<< ": " <<ans[((st.st_mode & S_IXOTH) == S_IXOTH)]<<endl;
}

bool isReverse(char buff1[], char buff2[], long long len) {
    for(long long i = 0; i < len; i++) {
        if (buff1[i] != buff2[len - i - 1]) {
            return false;
        }
    }
    return true;
}

long long getFileLength(int fd) {
    long long pos = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return pos;
}

bool whetherFileContentReversed(char file1Name[], char file2Name[]) {
    int file1Desc = open(file1Name, O_RDONLY);
    int file2Desc = open(file2Name, O_RDONLY);
    if (getFileLength(file1Desc) != getFileLength(file2Desc)) {
        return false;
    }
    long long totalLength = lseek(file2Desc, 0, SEEK_END);
    long long endIdx = totalLength - 1;
    long long content = 0;
    while(content != totalLength) {
        long long len;
        if (totalLength - content > BATCH_SIZE) {
            len = BATCH_SIZE;
        } else {
            len = totalLength - content;
        }
        char buff1[len];
        char buff2[len];
        read(file1Desc, buff1, len);
        long long offset = endIdx - len + 1;
        endIdx = offset - 1;
        lseek(file2Desc, offset, SEEK_SET);
        read(file2Desc, buff2, len);
        content += len;
        if (!isReverse(buff1, buff2, len)) {
            return false;
        }
    }
    return true;
}

int main(int argumentCount, char *argumentVector[]) {
    if (argumentCount < 4) {
        cout<<"Provide valid arguments\n";
        exit(0);   
    }
    struct stat st;
    stat(argumentVector[3], &st);
    cout<<"Directory is created: "<< (S_ISDIR(st.st_mode) ? "Yes" : "No")<<endl;
    cout<<"Whether file contents are reversed in newfile: "<< (whetherFileContentReversed(argumentVector[1], argumentVector[2]) ? "Yes" : "No")<<endl;
    getInformation(argumentVector[1], 0);
    getInformation(argumentVector[2], 1);
    getInformation(argumentVector[3], 2);
    return 0;
}