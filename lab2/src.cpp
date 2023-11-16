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
    // unsigned int seed = (unsigned int)time(NULL);
    //cout << "radius: " << radius << endl;
    for(int i = 0; i < data->total_points; ++i) {
        
        double min = -1 * radius;
        double max = radius;
        int precision = 3;
        
        double x, y;
        
        x = rand() % (int)pow(10, precision);
        x = min + (x / pow(10, precision)) * (max - min);
        
        y = rand() % (int)pow(10, precision);
        y = min + (y / pow(10, precision)) * (max - min);
        // double x = (((double)rand_r(&seed) / RAND_MAX) * 2 * radius) - radius;
        // double y = (((double)rand_r(&seed) / RAND_MAX) * 2 * radius) - radius;

        // cout << "x: " << x << "y: " << y << endl;

        if(x * x + y * y <= radius * radius) {
            data->in_points++;
        }
    }
    // cout << data->total_points << " " << data->in_points << endl;
    pthread_exit(NULL);
}
