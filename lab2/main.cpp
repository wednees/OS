// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <time.h>
// #include <sys/time.h>
// #include <math.h>

// typedef struct ThreadData {
//     int in_points;
//     int total_points;
//     double radius;
// } Spot;

// void* monteCarlo(void* arg) {
//     struct ThreadData* th_data = (struct ThreadData*)arg;
//     double radius = th_data->radius; 
//     unsigned int seed = (unsigned int)time(NULL);

//     for(int i = 0; i < th_data->total_points; ++i) {
//         double x = (((double)rand_r(&seed) / RAND_MAX) * 2 * radius) - radius;
//         double y = (((double)rand_r(&seed) / RAND_MAX) * 2 * radius) - radius;


//         if(x * x + y * y <= radius * radius) {
//             th_data->in_points++;
//         }
//     }
    
//     pthread_exit(NULL);
// }

#include "include.hpp"

int main(int argc, char* argv[]) {
    if(argc != 4) {
        cout << "Usage: " << argv[0] << " <radius> <number of threads> <number of points (increses accuracy)>" << endl;
        // printf("Usage: %s <radius> <number of th>\n", argv[0]);
        exit (-1);
    }

    double radius = atof(argv[1]);
    int number_of_threads = atoi(argv[2]);
    int points = atoi(argv[3]);

    pthread_t* th = new pthread_t [number_of_threads];
    Data* th_data = new Data [number_of_threads];
    int points_per_thread = points / number_of_threads;

    struct timeval start, end;
    int seconds, micro_seconds;

    gettimeofday(&start, NULL);

    for(int i = 0; i < number_of_threads; ++i) {
        th_data[i].in_points = 0;
        th_data[i].total_points = points_per_thread;
        th_data[i].radius = radius;
        if (pthread_create(&th[i], NULL, monte_carlo, (void*)&th_data[i]) != 0){
            perror("Error occured while creating a thread!");
            exit (-1);
        }
    }

    for(int i = 0; i < number_of_threads; ++i) {
        if (pthread_join(th[i], NULL) != 0){
            perror("Error occured while joining a thread!");
            exit (-1);
        }
    }

    gettimeofday(&end, NULL);

    int points_inside_circle = 0;
    for(int i = 0; i < number_of_threads; ++i) {
        points_inside_circle += th_data[i].in_points;
    }
   
   // printf("%d   ,   %.6lf\n", points_inside_circle, (double)points_inside_circle / points);
    double rel = (double) points_inside_circle / (double) points;
    double area_of_square = 4 * radius * radius;
    // cout << fixed << setprecision(3) << rel << " " << area_of_square << endl;
    double area_of_circle = area_of_square * rel ;

    cout << fixed << setprecision(3) << "Area of the circle: " << area_of_circle << "; Radius: " << radius << endl;
    // printf("Area of the cirlce with radius %.3lf: %.3lf \n", radius, area_of_circle);

    seconds = end.tv_sec - start.tv_sec;
    micro_seconds = end.tv_usec - start.tv_usec;
    double time = seconds + micro_seconds;
    cout << fixed << setprecision(3) << "Execution time: " << time << "ms" << endl;
    // printf("Execution time: %.3f ms\n", time);

    delete(th);
    delete(th_data); 
    return 0;
}
