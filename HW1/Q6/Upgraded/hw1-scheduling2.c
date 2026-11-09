#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREADS 8

typedef struct {
    int id;
    int priority;      // lower is more important
    int burst_time;    // CPU burst time
    int remaining;      // remaining time
    int finish_time;    // completion time 
} thread_info_t;

thread_info_t threads[NUM_THREADS];
int elapsed_time = 0;
int last_thread = -1;

// Global scheduler lock: only one thread "on CPU" at a time
pthread_mutex_t sched_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  sched_cond = PTHREAD_COND_INITIALIZER;
int current_running_tid = -1;   // the id of thread allowed to run

int pick_next() {
    for (int i = 0; i < NUM_THREADS; i++) {
        int next = (last_thread + 1 + i) % NUM_THREADS;

        if (threads[next].remaining > 0) {
            last_thread = next;
            return next;
        }
    }

    return -1;
}

void schedule() {
    pthread_mutex_lock(&sched_lock);
    current_running_tid = pick_next();
    pthread_cond_broadcast(&sched_cond);
    pthread_mutex_unlock(&sched_lock);
}

void *thread_func(void *arg) {
    thread_info_t *t = (thread_info_t *)arg;

    while (1) {
        pthread_mutex_lock(&sched_lock);

        while (current_running_tid != t->id && t->remaining > 0)
            pthread_cond_wait(&sched_cond, &sched_lock);

        if (t->remaining <= 0) {
            pthread_mutex_unlock(&sched_lock);
            break;
        }

        printf("Thread %d runs for 1 unit\n", t->id);

        t->remaining--;
        elapsed_time++;

        if (t->remaining == 0) {
            t->finish_time = elapsed_time;
        }

        current_running_tid = pick_next();
        pthread_cond_broadcast(&sched_cond);

        pthread_mutex_unlock(&sched_lock);
    }

    return NULL;
}

int main() {
    pthread_t tids[NUM_THREADS];
    int prios[]  = {3, 3, 2, 2, 2, 6, 7, 1}; // lower has higher priority
    int bursts[] = {7, 6, 2, 8, 7, 1, 1, 9};

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].id = i;
        threads[i].priority = prios[i];
        threads[i].burst_time = threads[i].remaining = bursts[i];
        pthread_create(&tids[i], NULL, thread_func, &threads[i]);
    }

    sleep(1);       
    schedule();     // kick off scheduling

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(tids[i], NULL);
    

    // TODO: print each thread's waiting time and turnaround time.
    
    
    int total_waiting = 0;
    int total_turnaround = 0;

    for (int i = 0; i < NUM_THREADS; i++) {

        int turnaround_time = threads[i].finish_time;
        int waiting_time = turnaround_time - threads[i].burst_time;

        printf("Thread %d: Waiting = %d, Turnaround = %d\n", i, waiting_time, turnaround_time);

        total_waiting += waiting_time;
        total_turnaround += turnaround_time;
    }
    

    // TODO: print the average waiting time and turnaround time.
    printf("Average waiting time: %.2f\n", (double)total_waiting / NUM_THREADS);

    printf("Average turnaround time: %.2f\n", (double)total_turnaround / NUM_THREADS);
    

    return 0;
}

