#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// Nb threads, ids, loads and delays for the threads
#define NBTHREADS 2
int ids[NBTHREADS] = {0,1};
int loads[NBTHREADS] = {4, 4};
int delays[NBTHREADS] = {1,2};
int sched_pri_vals[NBTHREADS] = {1,1};

// Pthread
pthread_barrier_t barrier;
pthread_t tid[NBTHREADS];
struct sched_param sched_params[NBTHREADS];
#define SCHED_TYPE SCHED_RR

// Time measuring
struct timespec start, end;


//Thread, simulates load and calculates response time
void * thread(void * arg) {
    int id = *((int *) arg);
    pthread_barrier_wait(&barrier); // Start synchronously

    while (1) {

        clock_gettime(CLOCK_MONOTONIC, &start);

        //printf("Starting task %d (load: %d | delay %d)", id, loads[id], delays[id]);
        for (int i = 0; i < 5; i++) {
            //printf(" . ");
            //fflush(stdout); // Output immediately
            for (int j = 0; j < 100000000*loads[id]; j++) {
                // Load 
            } 
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);

        long response_time = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_nsec - start.tv_nsec) / 1000L;
        printf("%d Done (response time: %ld ms)\n", id,  response_time /1000L);
         fflush(stdout); // Output immediately

        sleep(delays[id]);

    }
}



int main(void) {
    pthread_barrier_init(&barrier, NULL, NBTHREADS);

    // Create threads
    for (int i = 0; i < NBTHREADS; i++) {
        if(pthread_create(&tid[i], NULL, thread, (void *) &ids[i]))
            printf("Thread creation fail %d!\n", i);
        }

    // Set schedule priorities
    for (int i = 0; i < NBTHREADS; i++) {
        sched_params[i].sched_priority = sched_pri_vals[i];
        pthread_setschedparam(tid[i], SCHED_TYPE, &sched_params[i]);
    }


    // Wait for thread completion
    for (int i = 0; i < NBTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}