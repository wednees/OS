#include <unistd.h>
#include <iostream>

using namespace std;

void create_pipe (int fd[2]){
    if (pipe(fd) == -1){
        perror("Error occured while creating a pipe");
        exit (-1);
    }
}

int create_process(){
    int id = fork();
    if (id == -1){
        perror("Error occured while creating a procces");
        exit (-1);
    }
    return id;
}