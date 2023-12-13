#include <iostream>
#include <dlfcn.h>
#include <string>

using namespace std;

char dl1_name[] = "libprime_count_dyn.so";
char dl2_name[] = "libnum_sys_dyn.so";

char dl1_alg1[] = "prime_count_naive";
char dl1_alg2[] = "prime_count_eratosthenes";

char dl2_alg1[] = "translation_binary";
char dl2_alg2[] = "translation_trinity";

int main(){
    void* dl1_h = dlopen(dl1_name, RTLD_LAZY);
    void* dl2_h = dlopen(dl2_name, RTLD_LAZY);

    if(!dl1_h){
        perror("Error occured while opening first dynamic library");
        exit (-1);
    }
    
    if(!dl2_h){
        perror("Error occured while opening second dynamic library");
        exit (-1);
    }

    int alg = 1;

    while(true){
        cout << "List of available commands:" << endl;
        cout << "0. Change algorithm" << endl;
        cout << "1. Count the amount of prime numbers in [A, B]" << endl;
        cout << "2. Translate number into other number system" << endl;
        cout << endl;
        cout << "Enter your command (0, 1, 2): ";

        int command, a, b;
        long x;
        int dl1_res;
        string dl2_res;

        if(!(cin >> command)){
            cout << "Exiting the programm..." << endl;
            dlclose(dl1_h);
            dlclose(dl2_h);
            break;
        }

        const char* error;

        switch (command) {
        case 0:
            alg = (alg + 1) % 2;
            cout << "Settings changed." << endl;
            if (alg == 1){
                cout << "Now naive algorithm is used for counting prime numbers." << endl;
                cout << "Program will translate numbers into binary number system." << endl;
            }
            else{
                cout << "Now eratosthenes sieve is used for counting prime numbers." << endl;
                cout << "Program will translate numbers into trinity number system." << endl;
            }
            break;
        
        
        
        case 1:
            int (*dl1_func) (int, int);
            if (alg == 1){
                dl1_func = (int (*) (int, int))dlsym(dl1_h, dl1_alg1);
            }
            else{
                dl1_func = (int (*) (int, int))dlsym(dl1_h, dl1_alg2);
            }

            error = dlerror();
            if (error != NULL){
                perror("Error occured while trying to find function in the first library");
                dlclose(dl1_h);
                exit (-1);
            }

            cout << endl;
            cout << "Enter A and B for [A, B]: ";
            cin >> a >> b;
            dl1_res = (*dl1_func)(a, b);
            cout << endl;
            cout << "Number of prime numbers: " << dl1_res << endl;

            break;

        

        case 2:
            string (*dl2_func) (long);
            if (alg == 1){
                dl2_func = (string (*) (long))dlsym(dl2_h, dl2_alg1);
            }
            else{
                dl2_func = (string (*) (long))dlsym(dl2_h, dl2_alg2);
            }

            error = dlerror();
            if (error != NULL){
                perror("Error occured while trying to find function in the second library");
                dlclose(dl2_h);
                exit (-1);
            }

            cout << endl;
            cout << "Enter a number for translation: ";
            cin >> x;
            dl2_res = (*dl2_func)(x);
            cout << endl;
            cout << "Translated number: " << dl2_res << endl;

            break;



        default:
            break;
        }

        cout << endl;
    }

    return 0;
}