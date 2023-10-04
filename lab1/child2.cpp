#include <iostream>
#include <unistd.h>
#define MAX_LEN 1000

using namespace std;

void delete_double_spaces(char line[], int size) {
    int new_index = 0;
    bool previous_space = false;

    for (int i = 0; i < size; i++) {
        if (line[i] != ' ') {
            line[new_index] = line[i];
            new_index++;
            previous_space = false;
        } else {
            if (!previous_space) {
                line[new_index] = line[i];
                new_index++;
                previous_space = true;
            }
        }
    }

    line[new_index] = '\0';  // Добавляем символ конца строки
}

int main() {
    char line[MAX_LEN];
    read(STDIN_FILENO, line, MAX_LEN);
    delete_double_spaces(line, MAX_LEN);
    //cout << line << endl;
    write(STDOUT_FILENO, line, MAX_LEN);

    return 0;
}