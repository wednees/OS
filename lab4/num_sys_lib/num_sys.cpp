#include "num_sys.hpp"

string translation_binary(long x){
    long num = x;
    int k = 0;
    while(num >= (long)pow(2, k)){
        k++;
    }
    k--;

    string num_bin;

    while(k >= 0){
        if (num >= (long)pow(2, k)){
            num = num - (long)pow(2, k);
            num_bin.push_back('1');
        }
        else{
            num_bin.push_back('0');
        }
        k--;
    }

    return num_bin;
}

string translation_trinity(long x){
    long num = x;
    int k = 0;
    while(num >= (long)pow(3, k)){
        k++;
    }
    k--;

    string num_trinity;

    while(k >= 0){
        if (num >= 2 * (long)pow(3, k)){
            num = num - 2 * (long)pow(3, k);
            num_trinity.push_back('2');
        }
        else if (num >= (long)pow(3, k)){
            num = num - (long)pow(3, k);
            num_trinity.push_back('1');
        }
        else{
            num_trinity.push_back('0');
        }
        k--;
    }

    return num_trinity;
}