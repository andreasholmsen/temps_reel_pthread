#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// Nb threads, ids, loads and delays for the threads
#define NBTHREADS 2
int ids[NBTHREADS] = {0,1};
int loads[NBTHREADS] = {1, 1};
int delays[NBTHREADS] = {1,2};

// Pthread
pthread_barrier_t barrier;
pthread_t tid[NBTHREADS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * thread(void * arg) {
    int id = *((int *) arg);

    pthread_barrier_wait(&barrier); // Start synchronously

    while (1) {

        pthread_mutex_lock(&mutex); // "Lock processor"

        printf("Starting task %d (load: %d | delay %d)", id, loads[id], delays[id]);
        for (int i = 0; i < 5; i++) {
            usleep(100000*loads[id]);
            printf(" . ");
            fflush(stdout); // Output immediately
        }
        printf("Done\n");
        pthread_mutex_unlock(&mutex);
        
        sleep(delays[id]);
    }
}



int main(void) {
    pthread_barrier_init(&barrier, NULL, 2);


    for (int i = 0; i < NBTHREADS; i++) {
        if(pthread_create(&tid[i], NULL, thread, (void *) &ids[i]))
            printf("Thread creation fail %d!\n", i);
        }

    for (int i = 0; i < NBTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}