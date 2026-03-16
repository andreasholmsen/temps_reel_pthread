#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

void generic_load(int load) {
    int multiplyer = 1000;

    for ( int i =0; i < load *multiplyer; i ++) {
        usleep(100);
        if (i == load*multiplyer - 1) printf("Process with load %d finished!\n", load);
    }
}


void real_time_task() {
    struct timespec ts_intime;
    struct timespec ts_outtime;
    clock_gettime(0, &ts_intime);


    // Work
    generic_load(20);
    generic_load(30);



    clock_gettime(0, &ts_outtime);

    // Calculate time taken
    unsigned long int time_taken_s = ts_outtime.tv_sec - ts_intime.tv_sec;
    unsigned long int time_taken_ms = ( ((unsigned long int)ts_outtime.tv_nsec)  - ((unsigned long int) ts_intime.tv_nsec)) / 1000000;


    printf("Time taken : %ld.%ld s\n", time_taken_s, time_taken_ms);
}



int main (void) {

    real_time_task();


}