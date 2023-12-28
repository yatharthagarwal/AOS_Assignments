#include <fcntl.h>
#include<errno.h>
#include<iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include<string.h>

using namespace std;

const int BATCH_SIZE = 65536;    // modifiable batch size
long long contentProcessed = 0;

int openFile(char *fileNamePtr, int flag, int mod = -1) {
    int fileDescriptor;
    if (mod != -1) {
        fileDescriptor = open(fileNamePtr, flag, mod);
    } else {
        fileDescriptor = open(fileNamePtr, flag); 
    }
    if (fileDescriptor == -1) {                         // error handling if there is an error while opening file
        perror("ERROR: ");
        exit(0);
    }
    return fileDescriptor;
}

long long readFile(int fileDescriptor, char buffer[], long long contentLength) {
    long long length = read(fileDescriptor, buffer, contentLength);
    if (length == -1) {               // if error while reading file then returns -1
        perror("ERROR: ");   
        exit(0); 
    }
    if (length == 0) {
        cout<<"Warning: Reached EOF\n";
    }
    return length;
}

void reverse(char input[], long long length) {
    long long idx = 0;
    long long mid = length/2;
    while(idx < mid) {
        char temp = input[idx];
        input[idx] =  input[length - idx -1];
        input[length - idx -1] = temp;
        idx++;
    }
}

// void setFileAttributes(char *fileNamePtr, struct stat *fileAttributes) {
//     stat(fileNamePtr, fileAttributes);
// }

// void setOutputFileName(char *inputFileNamePtr, char *outputFilePtr, char *directory) {
//     int i = 0;
//     for(int j = 0; directory[j] != '\0'; j++, i++) {
//         outputFilePtr[i] = directory[j];
//     }
//     outputFilePtr[i++] = '2';
//     outputFilePtr[i++] = '_';
//     for(int j = 2; inputFileNamePtr[j] != '\0'; j++, i++) {
//         outputFilePtr[i] = inputFileNamePtr[j];
//     }
//     outputFilePtr[i] = '\0';
// }

void getPercentage(long long value, long long total) {
        long long percentage = (value/(float)total) * 100;
        cout<<'\r';
        cout<<percentage<<'%';
        return;
} 

void readFromEndOfFile(int ifd, int ofd, long long startIdx, long long endIdx, long long totalLength, bool rev = false) {
    if (startIdx > endIdx) {
        return;
    }
    if(startIdx < 0 || endIdx < 0 || startIdx > totalLength-1 || endIdx > totalLength-1) {
        cout<<"Indexed out of bounds. Please correct and try again!!\n";
        exit(0);
    }
    long long segmentLength = endIdx - startIdx + 1;
    long long totalBatches = (segmentLength/(double)BATCH_SIZE - segmentLength/BATCH_SIZE) > 0 ? segmentLength/BATCH_SIZE + 1 : segmentLength/BATCH_SIZE ;
    long long currentBatch = totalBatches;
    long long contentLength;                   // length of the content in a batch
    while(currentBatch > 0) {
        // read file  
        contentLength = (currentBatch == totalBatches) ? segmentLength - (BATCH_SIZE * (totalBatches - 1)) : BATCH_SIZE;
        char buffer[contentLength]; 
        long long offset = endIdx - contentLength + 1;
        lseek(ifd, offset, SEEK_SET);
        endIdx = offset - 1;
        readFile(ifd, buffer, contentLength);   // read data into the buffer
        if (rev) {
            reverse(buffer, contentLength);
        }
        write(ofd, buffer, contentLength);
        contentProcessed += contentLength;
        getPercentage(contentProcessed, totalLength);
        currentBatch--;
    }
}

int getNumeric(char arr[]) {
    int ans = 0;
    for(int i = 0; arr[i] != '\0'; i++) {
        ans = ans * 10 + (arr[i] - '0');
    }
    return ans;
}

void readFromFrontOfFile(int ifd, int ofd, long long startIdx, long long endIdx, long long totalLength) {
    if (startIdx > endIdx){
        return;
    }
    if ((startIdx < 0 || endIdx < 0 || startIdx > totalLength -1 || endIdx > totalLength - 1)) {
        cout<<"Indexed out of bounds. Please correct and try again!!\n";
        exit(0);
    }
    long long segmentLength = endIdx - startIdx + 1;
    lseek(ifd, startIdx, SEEK_SET);
    long long totalBatches = (segmentLength/(double)BATCH_SIZE - segmentLength/BATCH_SIZE) > 0 ? segmentLength/BATCH_SIZE + 1 : segmentLength/BATCH_SIZE;
    long long currentBatch = 1;
    long long contentLength = 0;
    while(currentBatch <= totalBatches) {
        contentLength = (currentBatch == totalBatches) ? segmentLength - (BATCH_SIZE * (totalBatches - 1)) : BATCH_SIZE; 
        char buffer[contentLength]; 
        readFile(ifd, buffer, contentLength);   // read data into the buffer
        write(ofd, buffer, contentLength);
        contentProcessed += contentLength;
        getPercentage(contentProcessed, totalLength);
        currentBatch++;
    }
}

int main(int argumentCount, char *argumentVector[]){
    if (argumentCount < 4) {
        cout<<"Incomplete args";
        exit(0);    
    }

    // open input file
    char *inputFileNamePtr = argumentVector[1];
    int inputFileDescriptor = openFile(inputFileNamePtr, O_RDONLY);

    // make directory
    int directoryCreated = mkdir("./Assignment1_2", S_IRUSR|S_IWUSR|S_IXUSR);
    if (directoryCreated == -1) {
        perror("WARNING: ");
        cout<<"Overiding files\n";
    }

    // open output file
    string outputFilePath = "./Assignment1_2/2_";
    char *p = strtok(inputFileNamePtr, "/");
    string inp;
    while(p!=NULL){
        inp=p;
        p=strtok(NULL, "/");
    }
    outputFilePath = outputFilePath.append(inp);
    char outputFileName[outputFilePath.length()];
    strcpy(outputFileName, outputFilePath.c_str());
    // setOutputFileName(inputFileNamePtr, outputFileName, directory);
    int outputFileDescriptor = openFile(outputFileName, O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);

    // struct stat fileAttributes;
    // setFileAttributes(inputFileNamePtr, &fileAttributes);
    long long totalLength = lseek(inputFileDescriptor, 0, SEEK_END);
    lseek(inputFileDescriptor, 0, SEEK_SET);
    readFromEndOfFile(inputFileDescriptor, outputFileDescriptor, 0, getNumeric(argumentVector[2]) - 1, totalLength, true);
    readFromFrontOfFile(inputFileDescriptor, outputFileDescriptor, getNumeric(argumentVector[2]), getNumeric(argumentVector[3]), totalLength);
    readFromEndOfFile(inputFileDescriptor, outputFileDescriptor, getNumeric(argumentVector[3]) + 1, totalLength - 1, totalLength, true);
    close(outputFileDescriptor);
    close(inputFileDescriptor);
    return 0;
}