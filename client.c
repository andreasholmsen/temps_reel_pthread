/*
 * Description:  A simple client program to connect to the TCP/IP server thanks to Darren Smith
 */

/*
 * Linux:   compile with gcc -Wall client.c -o client
 */

#include <stdio.h>
#include <string.h>
#include <semaphore.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>

#define SOCKET_PORT 10020
#define SOCKET_SERVER "127.0.0.1" /* local host */

int fd; 

// Thread parameters
#define NBTHREADS 1
pthread_t tid[NBTHREADS];
struct sched_param sched_params[NBTHREADS];
int sched_pri_vals[NBTHREADS] = {10}; // Read battery priority 10


pthread_mutex_t fd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t motor_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t battery_low;
sem_t sem;

void * read_battery(void * args) {
  char buffer[256];
  int n;
  while(1) {
    pthread_mutex_lock(&fd_mutex);
    send(fd,"B\n",strlen("B\n"),0);
    n = recv(fd,buffer,256,0);
    pthread_mutex_unlock(&fd_mutex);
    printf("Battery level: %s\n", buffer);
    if (atoi(buffer) < 10) { // If less than 10% battery
      sem_post(&battery_low); //trigger semaphore
      printf("BATTERY LOW\n");
    }

    memset(buffer, 0, sizeof(buffer));
    sleep(2);
  }
  
}

void * recharge(void * args) {
    while (1) {
    sem_wait(&battery_low);
    printf("RECHARGING\n");
    pthread_mutex_lock(&motor_mutex);
    send(fd,"M,0,0\n",strlen("M,0,0\n"),0);
    // TODO: BATTERY RECHARGE
    pthread_mutex_unlock(&motor_mutex);
  }
}

void * move_forward(void * args) {
  while(1) {
    pthread_mutex_lock(&motor_mutex);
    send(fd,"M,50,50\n",strlen("M,50,50\n"),0);
    pthread_mutex_unlock(&motor_mutex);
    usleep(1e5); // 100ms
  }
}

// Triggered by semaphore changing value ?
void turn_ninety_deg(sem_t * sem) {
  // Signal that it wants to take the semaphore
  sem_wait(sem);
  char buffer[256];
  send(fd,"T,1.57\n",strlen("T,1.57\n"),0);
  int n = recv(fd,buffer,256,0);
  // Increments
  sem_post(sem);
}

void read_distance(sem_t * sem) {
  // Signal that it wants to take the semaphore 
  sem_wait(sem);
  double left_sensor, right_sensor;
  char buffer[256];
  send(fd,"S\n",strlen("S\n"),0);
  int n = recv(fd,buffer,256,0);
  buffer[n] = '\0';
  printf("%s\n",buffer);
  printf("test1");
  
  sscanf(buffer,"S,%lf,%lf",&left_sensor,&right_sensor);
  printf("test2");
  printf("test %lf %lf", left_sensor, right_sensor);

  // Turn before crashing into an object
  if (left_sensor > 600 || right_sensor > 600) {
    // Increments
    sem_post(sem);
  }

  fflush(stdout);
} 


int main(int argc, char *argv[]) {
  struct sockaddr_in address;
  const struct hostent *server;
  int rc;
  //char buffer[256];

  /* create the socket */
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    printf("cannot create socket\n");
    return -1;
  }

  /* fill in the socket address */
  memset(&address, 0, sizeof(struct sockaddr_in));
  address.sin_family = AF_INET;
  address.sin_port = htons(SOCKET_PORT);
  server = gethostbyname(SOCKET_SERVER);

  if (server)
    memcpy((char *)&address.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
  else {
    printf("cannot resolve server name: %s\n", SOCKET_SERVER);
    close(fd);
    return -1;
  }

  /* connect to the server */
  rc = connect(fd, (struct sockaddr *)&address, sizeof(struct sockaddr));
  if (rc == -1) {
    printf("cannot connect to the server\n");
    close(fd);
    return -1;
  }

  fflush(stdout);

      // Start here

      // Initialize semaphore
      sem_init(&sem,0,1); // 0 = semaphore shared between the threads of a process
      sem_init(&battery_low, 0, 0); // Init semaphore
      pthread_create(&tid[0], NULL, read_battery, NULL); // read_battery thread
	
    // Set schedule priorities
      for (int i = 0; i < NBTHREADS; i++) {
          sched_params[i].sched_priority = sched_pri_vals[i];
          pthread_setschedparam(tid[i], SCHED_RR, &sched_params[i]);
      }
    
	
	 // Wait for thread completion
    for (int i = 0; i < NBTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    
      while(1) {
        read_distance();
        turn_ninety_deg();
        read_distance((void *) NULL);
      }


       // Start threads necessary

  close(fd);

  return 0;
}
