#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include "task.hpp"

using namespace std;

#define MAX_LEN 1000

int main(){
    int pipe_1_fd[2], pipe_2_fd[2], pipe_child_fd[2];
    create_pipe(pipe_1_fd);
    create_pipe(pipe_2_fd);
    create_pipe(pipe_child_fd);

    char line[MAX_LEN];
    char final_line[MAX_LEN];
    //cout << "Enter a line here: ";
    write(STDOUT_FILENO, "Enter a line here: ", 20);
    read(STDIN_FILENO, line, MAX_LEN);

    int id_1 = create_process();

    if (id_1 == 0){
        close(pipe_1_fd[1]);
        close(pipe_child_fd[0]);

        if (dup2(pipe_1_fd[0], STDIN_FILENO) == -1){
            perror("Error occured while executing dup2 function");
            exit (-1);
        }
        
        if (dup2(pipe_child_fd[1], STDOUT_FILENO) == -1){
            perror("Error occured while executing dup2 function");
            exit (-1);
        }
        
        //cout << "child1 executing" << endl;

        if (execl("child1", "child1", NULL) == -1){
            perror("Error occured while executing exec function");
            exit (-1);
        }
    } 
    else{
        close(pipe_1_fd[0]);
        write(pipe_1_fd[1], line, MAX_LEN);
        close(pipe_1_fd[1]);

        int id_2 = create_process();

        if (id_2 == 0){
            close(pipe_2_fd[0]);
            close(pipe_child_fd[1]);

            if (dup2(pipe_child_fd[0], STDIN_FILENO) == -1){
                perror("Error occured while executing dup2 function");
                exit (-1);
            }
            
            if (dup2(pipe_2_fd[1], STDOUT_FILENO) == -1){
                perror("Error occured while executing dup2 function");
                exit (-1);
            }

            //cout << "child2 executing" << endl;

            if (execl("child2", "child2", NULL) == -1){
                perror("Error occured while executing exec function");
                exit (-1);
            }
        }
        else{
            //wait(NULL);
            close(pipe_2_fd[1]);
            read(pipe_2_fd[0], final_line, MAX_LEN);
            close(pipe_2_fd[0]);

            cout<< "Final line: " << final_line;
        } 
    }

    return 0;
}