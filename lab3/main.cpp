#include "task.hpp"

using namespace std;

int main(){

    char line[MAX_LEN];
    char final_line[MAX_LEN];

    int lenght_of_line = inputer(line);

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd == -1){
        perror("Error occured while creating shared file!");
        exit (-1);
    }

    if(ftruncate(fd, MAX_LEN) == -1){
        perror("Error occured while setting the size of the file!");
        exit (-1);
    }
    
    char* shm_line = static_cast<char *>(mmap(NULL, MAX_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(shm_line == MAP_FAILED){
        perror("Error occured while mapping the memory!");
        exit (-1);
    }

    for(int i = 0; i < lenght_of_line; ++i){
        shm_line[i] = line[i];
    }

    int id_1 = create_process();

    if (id_1 == 0){

        if (execl("child1", "child1", SHM_NAME, NULL) == -1){
            perror("Error occured while executing exec function");
            exit (-1);
        }
    } 
    else{

        wait(NULL);

        int id_2 = create_process();

        if (id_2 == 0){

            if (execl("child2", "child2", SHM_NAME, NULL) == -1){
                perror("Error occured while executing exec function");
                exit (-1);
            }
        }
        else{

            wait(NULL);

            read(fd, final_line, lenght_of_line);

            for(int i = 0; i < lenght_of_line; ++i) {
                cout << final_line[i];
            }
            cout << endl;

            if(munmap(shm_line, MAX_LEN) != 0){
                perror("Error occured while unmapping the memory!");
                exit (-1);
            }

            shm_unlink(SHM_NAME);
            close(fd);
        } 
    }

    return 0;
}