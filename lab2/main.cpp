#include "include.hpp"

int main(int argc, char* argv[]) {
    if(argc != 4) {
        cout << "Usage: " << argv[0] << " <radius> <number of threads> <number of points (increses accuracy)>" << endl;
        exit (-1);
    }

    double radius = atof(argv[1]);
    int number_of_threads = atoi(argv[2]);
    int points = atoi(argv[3]);

    pthread_t* th = new pthread_t [number_of_threads];
    Data* th_data = new Data [number_of_threads];
    int points_per_thread = points / number_of_threads;

    clock_t start = clock();

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

    clock_t end = clock();;

    int points_inside_circle = 0;
    for(int i = 0; i < number_of_threads; ++i) {
        points_inside_circle += th_data[i].in_points;
    }
   
    double rel = (double) points_inside_circle / (double) points;
    double area_of_square = 4 * radius * radius;
    double area_of_circle = area_of_square * rel ;

    cout << fixed << setprecision(3) << "Area of the circle: " << area_of_circle << "; Radius: " << radius << endl;

    double time = (double)(end - start) / ((double) CLOCKS_PER_SEC);

    cout << fixed << setprecision(3) << "Execution time: " << time << "ms" << endl;

    delete(th);
    delete(th_data); 
    return 0;
}
