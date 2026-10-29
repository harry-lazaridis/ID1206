#include <pthread.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define N 1000301

#define ELAPSED(t0, t1) (((t1).tv_sec - (t0).tv_sec) + ((t1).tv_nsec - (t0).tv_nsec) / 1e9)

int num_threads = 0;
     
void *thread_func(); /* the thread function */

typedef struct {
    int id;
    float *array;
    int start, end;
    double *partial_sums;
} thread_arg_t;

//1,000,301 amount of floats, ranging from [0, 1].

int main(int argc, char *argv[])
{
    num_threads = atoi(argv[1]);
    //clock_t t;
    struct timespec t0, t1; 
    srand(time(0));

    /* Initialize an array of random values */
    float *array = malloc(sizeof(float) * N);
    for (int i = 0; i < N; i++) {
        array[i] = (float)rand() / RAND_MAX;
    }

    /* Perform Serial Sum */
    double  sum_serial = 0.0;
    double time_serial = 0.0;
    //Timer Begin
    //t = clock();
    clock_gettime(CLOCK_MONOTONIC, &t0); 

    for (int i = 0; i < N; i++) {
        sum_serial += array[i];
    }

    //t = clock() - t;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    //time_serial = ((double)t)/CLOCKS_PER_SEC;
    time_serial = ELAPSED(t0, t1);

    //Timer End
    printf("Serial Sum = %f, time = %.6f \n", sum_serial, time_serial);

    /* Create a pool of num_threads workers and keep them in workers */ 
    pthread_t *workers = malloc(sizeof(pthread_t) * num_threads);    
    double time_parallel = 0.0;
    double sum_parallel = 0.0;

    //Timer Begin
    //t = clock();
    clock_gettime(CLOCK_MONOTONIC, &t0);

    int chunk = N / num_threads;

    thread_arg_t *args = malloc(num_threads * sizeof(thread_arg_t));
    double *partial_sums = malloc(num_threads * sizeof(double));

    for (int i = 0; i < num_threads; i++) {
        args[i].id = i;
        args[i].array = array;
        args[i].start = i * chunk;
        args[i].end = (i == num_threads - 1) ? N : (i + 1) * chunk;
        args[i].partial_sums = partial_sums;
        pthread_create(&workers[i], NULL, thread_func, &args[i]);
    }
    
    for (int i = 0; i < num_threads; i++) 
        pthread_join(workers[i], NULL);

    for (int i = 0; i < num_threads; i++)
        sum_parallel += partial_sums[i];

    //Timer End
    //t = clock() - t;
    //time_parallel = ((double)t)/CLOCKS_PER_SEC;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    time_parallel = ELAPSED(t0, t1); 
    printf("Parallel Sum = %f, time = %.6f \n", sum_parallel, time_parallel);
    
    /*free up resources properly */
    free(array);
    free(workers);
    free(args);
    free(partial_sums); 
}

void *thread_func(void *arg) { 
    /* Assign each thread an id so that they are unique in range [0, num_thread -1 ] */
    thread_arg_t *a = (thread_arg_t *)arg;

    double my_sum = 0.0;
    for (int i = a->start; i < a->end; i++) {
        my_sum += a->array[i];
    }

    a->partial_sums[a->id] = my_sum;

    printf("Thread %d sum = %f\n", a->id, my_sum);
    pthread_exit(0);
}