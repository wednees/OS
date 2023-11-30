#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAX_LEN 1000
#define SHM_NAME "IPC_LINE"

int create_process();
int inputer(char line[]);