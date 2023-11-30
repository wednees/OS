#include "task.hpp"
using namespace std;

int create_process(){
    int id = fork();
    if (id == -1){
        perror("Error occured while creating a procces");
        exit (-1);
    }
    return id;
}

int inputer(char line[]){
    char c;
    int len = 0;

    while (((c = getchar()) != '\n') && (len < MAX_LEN)){
        line[len] = c;
        len++;
    }

    return len;
}