#include <unistd.h>
#include <iostream>

using namespace std;

#define MAX_LEN 1000

void make_upper(char line[], int size){
    for (int i = 0; i < size; ++i){
        if ((line[i] >= 'a') && (line[i] <= 'z')){
            line[i] = (char)((int)line[i] - ('a' - 'A'));
        }
    }
}

int main() {
    char line[MAX_LEN];
    read(STDIN_FILENO, line, MAX_LEN);
    make_upper(line, MAX_LEN);
    //cout << line << endl;
    write(STDOUT_FILENO, line, MAX_LEN);

    return 0;
}