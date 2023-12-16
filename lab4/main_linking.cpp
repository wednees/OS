#include <iostream>
#include <string>

#include "prime_count.hpp"
#include "num_sys.hpp"

using namespace std;

int main(){

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
        int res1;
        string res2;

        if(!(cin >> command)){
            cout << "Exiting the programm..." << endl;
            break;
        }

        switch (command) {
        case 0:
            alg = (alg + 1) % 2;
            cout << "Settings changed" << endl;
            if (alg == 1){
                cout << "Now naive algorithm is used for counting prime numbers." << endl;
                cout << "Program will translate numbers into binary number system" << endl;
            }
            else{
                cout << "Now eratosthenes sieve is used for counting prime numbers." << endl;
                cout << "Program will translate numbers into trinity number system" << endl;
            }
            break;
        
        
        
        case 1:

            cout << endl;
            cout << "Enter A and B for [A, B]: ";
            cin >> a >> b;
            
            if(alg == 1){
                res1 = prime_count_naive(a, b);
            }
            else{
                res1 = prime_count_eratosthenes(a, b);
            }

            cout << endl;
            cout << "Number of prime numbers: " << res1 << endl;

            break;

        

        case 2:

            cout << endl;
            cout << "Enter a number for translation: ";
            cin >> x;

            if (alg == 1){
                res2 = translation_binary(x);
            }
            else{
                res2 = translation_trinity(x);
            }

            cout << endl;
            cout << "Translated number: " << res2 << endl;

            break;



        default:
            break;
        }

        cout << endl;
    }

    return 0;
}