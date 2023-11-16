#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "include.hpp"

void* monte_carlo(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    double radius = data->radius; 

    for(int i = 0; i < data->total_points; ++i) {
        
        double min = -1 * radius;
        double max = radius;
        int precision = 3;
        
        double x, y;
        
        x = rand() % (int)pow(10, precision);
        x = min + (x / pow(10, precision)) * (max - min);
        
        y = rand() % (int)pow(10, precision);
        y = min + (y / pow(10, precision)) * (max - min);

        if(x * x + y * y <= radius * radius) {
            data->in_points++;
        }
    }
    pthread_exit(NULL);
}
