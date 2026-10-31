#include <pthread.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define ARRAY_SIZE 1000301
     
int num_threads = 0;
float *array;
double *partial_sums;
     
void *thread_func(); /* the thread function */

int main(int argc, char *argv[])
{
     num_threads = atoi(argv[1]);


     /* Initialize an array of random values */ 

     array = malloc(ARRAY_SIZE * sizeof(float));

     if (array == NULL) {
     perror("malloc");
     return 1;
     }

     for (int i = 0; i < ARRAY_SIZE; i++) {
          array[i] = (float)rand() / RAND_MAX;
     }

     /* Perform Serial Sum */
     float  sum_serial = 0.0;
     double time_serial = 0.0;
    
    
    
     //Timer Begin

     struct timespec start, end;

     clock_gettime(CLOCK_MONOTONIC, &start);

     for (int i = 0; i < ARRAY_SIZE; i++) {
          sum_serial += array[i];
     }

     



     //Timer End

     clock_gettime(CLOCK_MONOTONIC, &end);

     time_serial = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

     printf("Serial Sum = %f, time = %.6f seconds\n",sum_serial, time_serial);

     

     /* Create a pool of num_threads workers and keep them in workers */ 
     pthread_t *workers = malloc(num_threads * sizeof(pthread_t));
     int *ids = malloc(num_threads * sizeof(int));
     partial_sums = calloc(num_threads, sizeof(double));




     double time_parallel = 0.0;
     double sum_parallel = 0.0;

     //Timer Begin

     clock_gettime(CLOCK_MONOTONIC, &start);

     for (int i = 0; i < num_threads; i++) { 

          ids[i] = i;
          pthread_attr_t attr;
          pthread_attr_init(&attr);
          pthread_create(&workers[i], &attr, thread_func, &ids[i]);
          pthread_attr_destroy(&attr);
     }
     
     for (int i = 0; i < num_threads; i++) {
          pthread_join(workers[i], NULL);
     }

     for (int i = 0; i < num_threads; i++) {
          sum_parallel += partial_sums[i];
     }

     

     //Timer End

     clock_gettime(CLOCK_MONOTONIC, &end);

     time_parallel = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

     printf("Parallel Sum = %f, time = %.6f seconds\n", sum_parallel, time_parallel);
     
     /*free up resources properly */
     free(workers);
     free(ids);
     free(partial_sums);
     free(array);
     
}

void *thread_func(void *arg) { 
     /* Assign each thread an id so that they are unique in range [0, num_thread -1 ] */
     int my_id = *(int *)arg;

     int start = (int)((long long)my_id * ARRAY_SIZE / num_threads);
     int end = (int)((long long)(my_id + 1) * ARRAY_SIZE / num_threads);

     /* Perform Partial Parallel Sum Here */
     float my_sum = 0.0;

     for (int i = start; i < end; i++) {
        my_sum += array[i];
     }

     partial_sums[my_id] = my_sum;

     printf("Thread %d sum = %f\n", my_id, my_sum);
     pthread_exit(0);
}