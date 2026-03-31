#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// Nb threads, ids, loads and delays for the threads
#define NBTHREADS 3
int ids[NBTHREADS] = {0,1, 2};
int loads[NBTHREADS] = {4, 4, 4};
int delays[NBTHREADS] = {2,2, 1};
int sched_pri_vals[NBTHREADS] = {1,10, 5};

// Pthread
pthread_barrier_t barrier;
pthread_t tid[NBTHREADS];
struct sched_param sched_params[NBTHREADS];
#define SCHED_TYPE SCHED_RR
pthread_mutex_t mutex;

// Time measuring
struct timespec start, end;


//Thread, simulates load and calculates response time
void * thread(void * arg) {
    int id = *((int *) arg);
    pthread_barrier_wait(&barrier); // Start synchronously

    while (1) {

        clock_gettime(CLOCK_MONOTONIC, &start);
        if (id ==2) {for(int i = 0; i < 1e6; i++) {

        }}
        if (id < 2) {pthread_mutex_lock(&mutex);} // Mutex for high and low priority task

        //printf("Starting task %d (load: %d | delay %d)", id, loads[id], delays[id]);
        for (int i = 0; i < 5; i++) {
            //printf(" . ");
            //fflush(stdout); // Output immediately
            for (int j = 0; j < 100000000*loads[id]; j++) {
                // Load 
            } 
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (id < 2) {pthread_mutex_unlock(&mutex);}

        long response_time = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_nsec - start.tv_nsec) / 1000L;
        //printf("%d Done (response time: %ld ms)\n", id,  response_time /1000L);
        printf("%d %ld\n", id,  response_time /1000L);
        fflush(stdout); // Output immediately

        sleep(delays[id]);

    }
}



int main(void) {
    pthread_barrier_init(&barrier, NULL, NBTHREADS);

    // Setup Mutex
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mutex, &attr);

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