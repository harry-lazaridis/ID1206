#include <pthread.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

int num_threads, array_size, num_bins;
float *array;
int *partial_histograms;

void *thread_func(void *arg) {
    int id = *(int *)arg;

    int start = (long long)id * array_size / num_threads;
    int end = (long long)(id + 1) * array_size / num_threads;

    for (int i = start; i < end; i++) {
        int bin = array[i] * num_bins;
        if (bin == num_bins) bin--;

        partial_histograms[id * num_bins + bin]++;
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) return 1;

    num_threads = atoi(argv[1]);
    array_size = atoi(argv[2]);
    num_bins = atoi(argv[3]);

    if (num_threads <= 0 || array_size <= 0 || num_bins <= 0)
        return 1;

    array = malloc((size_t)array_size * sizeof(float));
    int *serial = calloc(num_bins, sizeof(int));
    int *parallel = calloc(num_bins, sizeof(int));
    partial_histograms = calloc((size_t)num_threads * num_bins, sizeof(int));

    pthread_t *workers = malloc((size_t)num_threads * sizeof(pthread_t));
    int *ids = malloc((size_t)num_threads * sizeof(int));

    if (!array || !serial || !parallel || !partial_histograms ||
        !workers || !ids) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < array_size; i++)
        array[i] = (float)rand() / RAND_MAX;

    struct timespec start, end;

    // Serial histogram
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < array_size; i++) {
        int bin = array[i] * num_bins;
        if (bin == num_bins) bin--;
        serial[bin]++;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double serial_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Parallel histogram
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        ids[i] = i;
        pthread_create(&workers[i], NULL, thread_func, &ids[i]);
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(workers[i], NULL);

    for (int i = 0; i < num_threads; i++)
        for (int j = 0; j < num_bins; j++)
            parallel[j] += partial_histograms[i * num_bins + j];

    clock_gettime(CLOCK_MONOTONIC, &end);
    double parallel_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Print histograms
    printf("Bin\tSerial\tParallel\n");

    for (int i = 0; i < num_bins; i++)
        printf("%d\t%d\t%d\n", i, serial[i], parallel[i]);

    printf("Serial time: %.6f seconds\n", serial_time);
    printf("Parallel time: %.6f seconds\n", parallel_time);

    free(array);
    free(serial);
    free(parallel);
    free(partial_histograms);
    free(workers);
    free(ids);

    return 0;
}
