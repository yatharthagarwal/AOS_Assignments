#include <fcntl.h>
#include<errno.h>
#include<iostream>
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
//     outputFilePtr[i++] = '1';
//     outputFilePtr[i++] = '_';
//     for(int j = 2; inputFileNamePtr[j] != '\0'; j++, i++) {
//         outputFilePtr[i] = inputFileNamePtr[j];
//     }
//     outputFilePtr[i] = '\0';
// }

// void setOutputFileName(char *inputFileNamePtr, string outputFilePath, char *outputFilePtr) {
//     cout<<"in method\n";
//     char *p = strtok(inputFileNamePtr, "/.");
//     string inp;
//     while(p!=NULL){
//         inp=p;
//         p=strtok(NULL, "/");
//     }
//     cout<<"in method1\n";
//     // char *temp;
//     // strcpy(temp, inp.c_str());
//     outputFilePath = outputFilePath.append(inp);
//     char ss[outputFilePath.length()];
//     cout<<"in method2: "<<outputFilePath<<endl;
//     strcpy(ss, outputFilePath.c_str());
//     cout<<ss<<endl;
//     outputFilePtr = ss;
//     for(int i=0; outputFilePtr[i] != '\0'; i++){
//         cout<<outputFilePtr[i];
//     }
//     cout<<"end\n";
// }

void readFromEndOfFile(int ifd, char *inpFileNamePtr, int ofd, long long startIdx, long long endIdx, long long totalLength) {
    long long segmentLength = endIdx - startIdx + 1;
    long long totalBatches = 0;
    float batches = segmentLength/(float)BATCH_SIZE - segmentLength/BATCH_SIZE;
    if (batches > 0) {
        totalBatches = segmentLength/BATCH_SIZE + 1;
    } else {
        totalBatches = segmentLength/BATCH_SIZE;
    }
    long long currentBatch = totalBatches;
    long long contentLength;                   // length of the content in a batch
    while(currentBatch > 0) {
        // read file      
        if (currentBatch == totalBatches) {
            contentLength = segmentLength - (BATCH_SIZE * (totalBatches - 1));
        } else {
            contentLength = BATCH_SIZE;
        }
        contentProcessed += contentLength;
        char buffer[contentLength]; 
        lseek(ifd, -contentProcessed, SEEK_END);
        readFile(ifd, buffer, contentLength);   // read data into the buffer
        reverse(buffer, contentLength);
        write(ofd, buffer, contentLength);
        long long percentageCompleted = (contentProcessed/(float)totalLength) * 100;
        cout<<'\r';
        cout<<percentageCompleted;
        cout<<'%';
        currentBatch--;
    }
}

int main(int argumentCount, char *argumentVector[]){
    if (argumentCount < 2) {
        write(1, "Input file name should be provided\n", 35);
        exit(0);    
    }

    // open input file
    char *inputFileNamePtr = argumentVector[1];
    int inputFileDescriptor = openFile(inputFileNamePtr, O_RDONLY);

    // make directory
    int directoryCreated = mkdir("./Assignment1_1", S_IRUSR|S_IWUSR|S_IXUSR);
    if (directoryCreated == -1) {
        perror("WARNING: ");
        cout<<"Overiding files\n";
    }

    // open output file
    string outputFilePath = "./Assignment1_1/1_";
    char *p = strtok(inputFileNamePtr, "/");
    string inp;
    while(p!=NULL){
        inp=p;
        p=strtok(NULL, "/");
    }
    outputFilePath = outputFilePath.append(inp);
    char outputFileName[outputFilePath.length()];
    strcpy(outputFileName, outputFilePath.c_str());
    int outputFileDescriptor = openFile(outputFileName, O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);

    // struct stat fileAttributes;
    // setFileAttributes(inputFileNamePtr, &fileAttributes);
    long long totalLength = lseek(inputFileDescriptor, 0, SEEK_END);
    lseek(inputFileDescriptor, 0, SEEK_SET);
    readFromEndOfFile(inputFileDescriptor, inputFileNamePtr, outputFileDescriptor, 0, totalLength - 1, totalLength);
    close(outputFileDescriptor);
    close(inputFileDescriptor);
    return 0;
}