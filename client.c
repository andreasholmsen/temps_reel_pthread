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

#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define SOCKET_PORT 10020
#define SOCKET_SERVER "127.0.0.1" /* local host */

int fd; 

struct periodic_task;
void sleep_until_next_activation(struct periodic_task *tsk);
void timespec_add(struct timespec *a, struct timespec *b);

void * battery_init (void * args);
void battery_activation(struct periodic_task *tsk);
void * move_forward_init(void * args);
void move_forward_activation(struct periodic_task *tsk);
void * read_distance_init(void * args);
void read_distance_activation(struct periodic_task *tsk);

#define PERIOD_IN_NANOS (100UL * 1000000UL)

// Thread parameters
#define NBTHREADS 5
pthread_t tid[NBTHREADS];
struct sched_param sched_params[NBTHREADS];
int sched_pri_vals[NBTHREADS] = {1, 1, 1, 1 ,1}; // Scheduling priorities


pthread_mutex_t fd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t motor_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t battery_low;
sem_t sem_turn;

void * recharge(void * args) {
    while (1) {
    sem_wait(&battery_low);
    printf("RECHARGING\n");
    pthread_mutex_lock(&motor_mutex);
     pthread_mutex_lock(&fd_mutex);
    send(fd,"M,0,0\n",strlen("M,0,0\n"),0);
    pthread_mutex_unlock(&fd_mutex);
    sleep(10);
    pthread_mutex_unlock(&motor_mutex);
  }
}

// Triggered by semaphore changing value ?
void * turn_ninety_deg(void * args) {
  int i = 1;
  while(1) {
    // Signal that it wants to take the semaphore
    sem_wait(&sem_turn);
    char buffer[256];
     pthread_mutex_lock(&motor_mutex);
     pthread_mutex_lock(&fd_mutex);
     send(fd,"M,30,-30\n",strlen("M,30,-30\n"),0);
    pthread_mutex_unlock(&fd_mutex);
    usleep(150e3);
    send(fd,"M,0,0\n",strlen("M,0,0\n"),0);
    pthread_mutex_unlock(&motor_mutex);
  }
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

      // Initialize semaphore
      sem_init(&sem_turn,0,0);
      sem_init(&battery_low, 0, 0); // Init semaphore

      pthread_create(&tid[2], NULL, read_distance_init, NULL);
      pthread_create(&tid[0], NULL, battery_init, NULL); // read_battery thread
      pthread_create(&tid[1], NULL, move_forward_init, NULL);
      pthread_create(&tid[3], NULL, turn_ninety_deg, NULL);
     



    // Set schedule priorities
      for (int i = 0; i < NBTHREADS; i++) {
          sched_params[i].sched_priority = sched_pri_vals[i];
          pthread_setschedparam(tid[i], SCHED_RR, &sched_params[i]);
      }
    
	
	 // Wait for thread completion
    for (int i = 0; i < NBTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    
  close(fd);

  return 0;
}

 struct periodic_task { 
        unsigned long current_job_id; // sequence number of the current job
        struct timespec period;  // desired separation of consecutive activations
        struct timespec first_activation;  // time at which the task became fist operational
        struct timespec current_activation; // time at which the current instance was (supposed to be) activated
        int terminated;  // flag to let applications terminate themselves
};

void sleep_until_next_activation(struct periodic_task *tsk) {
	int err;
	do {
	  // perform an absolute sleep until tsk->current_activation
		err = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tsk->current_activation, NULL);
		// if err is nonzero, we might have woken up too early
	} while (err != 0 && errno == EINTR);
	assert(err == 0);
}



void timespec_add(struct timespec *a, struct timespec *b) {
	a->tv_sec += b->tv_sec;
	a->tv_nsec += b->tv_nsec;
	if (a->tv_nsec >= 1000000000UL) {
		a->tv_sec++;
		a->tv_nsec %= 1000000000UL;
	}
}










// Battery activation
void * battery_init(void * args) {
	int err;
	struct periodic_task battery;

	// to match the real-time theory, the job count starts at â€œ1â€
	battery.current_job_id = 0;

	// run until application logic tells us to shut down
	battery.terminated = 0;

	// note the desired period
	battery.period.tv_sec = 2;
	battery.period.tv_nsec = PERIOD_IN_NANOS;

	// record time of first job
	err = clock_gettime(CLOCK_MONOTONIC, &battery.first_activation);
	assert(err == 0);
	battery.current_activation = battery.first_activation;

	// execute a sequence of jobs until app shuts down (if ever)
	while (!battery.terminated) {
		// wait until release of next job
		sleep_until_next_activation(&battery);
		// call the actual application logic
		battery_activation(&battery);
		// advance the job count in preparation of the next job
		battery.current_job_id++;
		// compute the next activation time
		timespec_add(&battery.current_activation, &battery.period);
	}
  return NULL;
}

void * move_forward_init(void * args) {
	int err;
	struct periodic_task task;

	// to match the real-time theory, the job count starts at â€œ1â€
	task.current_job_id = 0;

	// run until application logic tells us to shut down
	task.terminated = 0;

	// note the desired period
	task.period.tv_sec = 2;
	task.period.tv_nsec = PERIOD_IN_NANOS;

	// record time of first job
	err = clock_gettime(CLOCK_MONOTONIC, &task.first_activation);
	assert(err == 0);
	task.current_activation = task.first_activation;

	// execute a sequence of jobs until app shuts down (if ever)
	while (!task.terminated) {
		// wait until release of next job
		sleep_until_next_activation(&task);
		// call the actual application logic
		move_forward_activation(&task);
		// advance the job count in preparation of the next job
		task.current_job_id++;
		// compute the next activation time
		timespec_add(&task.current_activation, &task.period);
	}
  return NULL;
}

// Battery activation
void * read_distance_init(void * args) {
	int err;
	struct periodic_task task;

	// to match the real-time theory, the job count starts at â€œ1â€
	task.current_job_id = 0;

	// run until application logic tells us to shut down
	task.terminated = 0;

	// note the desired period
	task.period.tv_sec = 0;
	task.period.tv_nsec = 5e8;

	// record time of first job
	err = clock_gettime(CLOCK_MONOTONIC, &task.first_activation);
	assert(err == 0);
	task.current_activation = task.first_activation;

	// execute a sequence of jobs until app shuts down (if ever)
	while (!task.terminated) {
		// wait until release of next job
		sleep_until_next_activation(&task);
		// call the actual application logic
		read_distance_activation(&task);
		// advance the job count in preparation of the next job
		task.current_job_id++;
		// compute the next activation time
		timespec_add(&task.current_activation, &task.period);
	}
  return NULL;
}




void battery_activation(struct periodic_task *tsk) {
  char buffer[256];
  int already_low = 0;
  pthread_mutex_lock(&fd_mutex);
  send(fd,"B\n",strlen("B\n"),0);
  recv(fd,buffer,256,0);
  pthread_mutex_unlock(&fd_mutex);
  float battery_level;
  sscanf(buffer, "B,%f", &battery_level);
  printf("Battery level: %s\n", buffer);
  fflush(stdout);
  if (battery_level < 50.0 && !already_low) { // If battery low
    already_low = 1;
    sem_post(&battery_low); //trigger semaphore
    printf("BATTERY LOW\n");
  } else {
    already_low = 0;
  }

  memset(buffer, 0, sizeof(buffer));
}


void move_forward_activation(struct periodic_task *tsk) {
    pthread_mutex_lock(&motor_mutex);
    pthread_mutex_lock(&fd_mutex);
    send(fd,"M,30,30\n",strlen("M,30,30\n"),0);
    pthread_mutex_unlock(&fd_mutex);
    pthread_mutex_unlock(&motor_mutex);
}


void read_distance_activation(struct periodic_task *tsk) {
  // Signal that it wants to take the semaphore 
  double left_sensor, right_sensor;
  char buffer[256];
  pthread_mutex_lock(&fd_mutex);
  send(fd,"S\n",strlen("S\n"),0);
  int n = recv(fd,buffer,256,0);
  pthread_mutex_unlock(&fd_mutex);
  buffer[n] = '\0';
  printf("%s\n",buffer);
  fflush(stdout);
  
  sscanf(buffer,"S,%lf,%lf",&left_sensor,&right_sensor);

  // Turn before crashing into an object
  if (left_sensor > 200 || right_sensor > 200) {
    // Increments
    printf("I have to turn\n");
    fflush(stdout);
    sem_post(&sem_turn);
  }

  
} 