#include "task.hpp"

using namespace std;

void make_upper(char line[], int size){
    for (int i = 0; i < size; ++i){
        if ((line[i] >= 'a') && (line[i] <= 'z')){
            line[i] = (char)((int)line[i] - ('a' - 'A'));
        }
    }
}

int main(int argc, char** argv) {
    
    if (argc != 2){
        perror("Argumenti blin polomani!");
        exit (-1);
    }

    int fd = shm_open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd == -1){
        perror("Error occured while opening the shared file!");
        exit (-1);
    }

    char* shm_line = static_cast<char *>(mmap(NULL, MAX_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (shm_line == MAP_FAILED){
        perror("Error occured while mapping the memory!");
        exit (-1);
    }

    make_upper(shm_line, MAX_LEN);;
    return 0;
}