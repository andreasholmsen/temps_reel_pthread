/*
 * Description:  A simple client program to connect to the TCP/IP server thanks to Darren Smith
 */

/*
 * Linux:   compile with gcc -Wall client.c -o client
 */

#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <time.h>
#include <assert.h>
#include <errno.h>

#define SOCKET_PORT 10020
#define SOCKET_SERVER "127.0.0.1" /* local host */

int fd; 

void read_battery() {
	send(fd,"B\n",strlen("B, 10\n"),0);

}

void turn_ninety_deg() {
  char buffer[256];
  send(fd,"T,1.57\n",strlen("T,1.57\n"),0);
  int n = recv(fd,buffer,256,0);
}

void read_distance() {
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
  fflush(stdout);
 }
  
  // Compare distance to a threshold
  // if (left_sensor > 0 && right_sensor > 0)






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

      // Set motor to 50%
      send(fd,"M,50,50\n",strlen("M,50,50\n"),0);
      turn_ninety_deg();
      send(fd,"M,100,100\n",strlen("M,100,100\n"),0);
    
      while(1) {
        read_distance();
      }

   
       // Start threads necessary
       
   
  
  while(1);
  

  close(fd);

  return 0;
}
