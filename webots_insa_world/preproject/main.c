#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_barrier_t barrier;
struct timespec ts_intime;
struct timespec ts_outtime[2]; // outtime


// SET LOAD
int load[2] = {15, 34};

void calculate_time(struct timespec outtime, struct timespec intime) {
    long sec = outtime.tv_sec - intime.tv_sec;
    long nsec = outtime.tv_nsec - intime.tv_nsec;

    // normalise if nsec negative
    if (nsec < 0) {
        sec -= 1;
        nsec += 1000000000L;
    }

    long ms = nsec / 1000000L;

    printf("Time taken: %ld.%ld s\n", sec, ms);
}

// Not used for multithreading
void generic_load(int load) {
    int multiplyer = 1000;

    for ( int i =0; i < load *multiplyer; i ++) {
        usleep(100);
        if (i == load*multiplyer - 1) printf("Process with load %d finished!\n", load);
    }
}

// Multithreading "generic work" with response time corresponding to load[id]
void * generic_load_barrier(void * ids) {
    int multiplyer = 1000;

    int id = *(int *) ids;

    pthread_barrier_wait(&barrier);

    for ( int i =0; i < load[id] *multiplyer; i ++) {
        usleep(100);
        if (i == load[id]*multiplyer - 1) printf("Process with load %d finished!\n", load[id]);
    }
    clock_gettime(0, &ts_outtime[id]);
}


// Main function
void real_time_task() {

    // Init barrier
    pthread_barrier_init(&barrier, NULL, 2);
    pthread_t t[2];
    int id[2];

    clock_gettime(0, &ts_intime);
    for(int i=0; i<2; i++)
      {
         id[i] = i;
         if(pthread_create(&t[i], NULL, generic_load_barrier, (void *) &id[i]))
            printf("Unable to create thread %d!\n", i);
      }

    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);

    // Calculate times taken
    for (int i = 0; i < 2; i++) {
        calculate_time(ts_outtime[i], ts_intime);
    }

}

int main (void) {

    real_time_task();


}