#include "prime_count.hpp"

int prime_count_naive(int a, int b){
    
    if ((a > b) || (a < 0) || (b < 0)){
        return -1;
    }

    bool flag = false;
    if (a == 1){
        flag = true;
    }

    int counter_global = 0;
    for(int i = a; i <= b; ++i){
        int counter = 0;
        for(int j = 2; j < i; ++j){
            if (i % j == 0){
                counter++;
            }
        }
        if (counter == 0){
            counter_global++;
        }
    }

    if (flag){
        counter_global--;
        return counter_global;
    }
    return counter_global;
}



int prime_count_eratosthenes(int a, int b){
    
    if((a > b) || (a < 0) || (b < 0)){
        return -1;
    }

    if(b == 1){
        return 0;
    }
    
    vector<int> sieve(b + 1, 1);
    sieve[0] = 0;
    sieve[1] = 0;

	for(int i = 2; i * i <= b; ++i){
		if(sieve[i] == 1){ 
			for(int j = i * i; j <= b; j += i){
				sieve[j] = 0;
			}
		}	
    }
    
    int counter = 0;
    for(int i = a; i <= b; ++i){
        if(sieve[i] == 1){
            counter++;
        }
    }

	return counter;
}