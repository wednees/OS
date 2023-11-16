#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

using namespace std;

typedef struct ThreadData {
    int in_points;
    int total_points;
    double radius;
} Data;

void* monte_carlo(void* arg);
