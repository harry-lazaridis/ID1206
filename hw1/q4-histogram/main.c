#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ELAPSED(t0, t1) (((t1).tv_sec - (t0).tv_sec) + ((t1).tv_nsec - (t0).tv_nsec) / 1e9)

typedef struct {
    int id;
    float *array;
    int start, end;
    int num_bins;
    int *partial; // num_threads * num_bins
} thread_arg_t;

void *thread_func(void *arg);

/**
    . Run: ./program 3 800 16, ./program 3 80000 16, and ./program 3 8000000 16. Include screenshots
    of your output here.
    2. Which data structures may have read access from multiple threads?
    3. Which data structures may have read and write access from multiple threads?
    4. Name one or more scheduling algorithm suitable for this program. Name one or more scheduling
    algorithm NOT suitable for this program. Explain why.

    2) [array, args] is accessed from all threads.
    3) [partial] is accessed and modified from all threads.
    4) 
        Scheduling

        Suitable:
            * Round-robin: Threats all workers equally, 
            so they make progress at the same rate and finish around the same time.
            Making no worker hold up the join.
        Not suitable:
            * Static priority scheduling: Giving different priority for workers making it possible
            to make a thread to have lower priority ending up delayed or starved while the others finish.

*/

int main(int argc, char *argv[]) {
   

    /*
        Create an array. The array length is given by argv[2]. The array should be initialized with random floats
        in the [0.0, 1.0] range.
        The master thread builds the histogram of the array in argv[3] bins. Print the histogram.  
    */ 

    struct timespec t0, t1;    

    srand(time(0));
    int num_bins = atoi(argv[3]);
    int sizeOfArray = atoi(argv[2]);
    int num_threads = atoi(argv[1]);

    float *array = malloc(sizeof(float) * sizeOfArray);
    for (int i = 0; i < sizeOfArray; i++) {
        array[i] = (float)rand() / RAND_MAX;
    }

    //Start time
    clock_gettime(CLOCK_MONOTONIC, &t0);

    int *hist = calloc(num_bins, sizeof(int));
    for (int i = 0; i < sizeOfArray; i++) {
        int bin = (int)(array[i] * num_bins);
        if (bin == num_bins) bin = num_bins - 1;
        hist[bin]++;
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    for (int i = 0; i < num_bins; i++) {
        printf("bin %2d: %d\n", i, hist[i]);
    }

    printf("Master thread, time = %.6f \n", ELAPSED(t0, t1));
    
    /*
        A group of worker threads are created. The number of threads is given by argv[1]. Each thread builds
        the histogram of a portion of the array. The final histogram is built by aggregating these partial
        histograms. Print the histogram 
        
        Antar att det ska vara vertikalt?

        0 - 0.x: ** (2)
        0 - 0.x: **** (4)
        0 - 0.x: * (1)
        0 - 1.x: *** (3)
    */

    pthread_t *workers = malloc(sizeof(pthread_t) * num_threads);
    int *partial = calloc(num_threads * num_bins, sizeof(int));
   
    int chunk = sizeOfArray / num_threads;
   

    clock_gettime(CLOCK_MONOTONIC, &t0);    
    
    thread_arg_t *args = malloc(num_threads * sizeof(thread_arg_t));
    for (int i = 0; i < num_threads; i++) {
        args[i].id = i;
        args[i].array = array;
        args[i].start = i * chunk;
        args[i].end = (i == num_threads - 1) ? sizeOfArray : (i + 1) * chunk;
        args[i].partial = partial;
        args[i].num_bins = num_bins;

        pthread_create(&workers[i], NULL, thread_func, &args[i]);
    }

    //clock_gettime(CLOCK_MONOTONIC, &t0);     

    for (int i = 0; i < num_threads; i++) {
        pthread_join(workers[i], NULL);
    }

    int *hist_par = calloc(num_bins, sizeof(int));
    for (int i = 0; i < num_threads; i++) {
        for (int j = 0; j < num_bins; j++) {
            hist_par[j] += partial[i * num_bins + j];
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    for (int i = 0; i < num_bins; i++) {
        printf("bin %2d: %d\n", i, hist[i]);
    }

    printf("Parallel time = %.6f\n", ELAPSED(t0, t1));

    free(array); 
    free(hist); 
    free(hist_par);
    free(partial); 
    free(workers); 
    free(args);

    return 0;
}

void *thread_func(void *arg) {
    thread_arg_t *a = (thread_arg_t *)arg;
    int *my_hist = a->partial + a->id * a->num_bins;

    for (int i = a->start; i < a->end; i++) {
        int bin = (int)(a->array[i] * a->num_bins);
        if (bin == a->num_bins) bin = a->num_bins - 1;
        my_hist[bin]++;
    }

    pthread_exit(NULL);
}